/*
 * cfm-controller.cc
 * Copyright (C) 2011 Adrian Perez <aperez@igalia.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "cfm-controller.h"
#include <dbus/dbus.h>
#include <dbus/dbus-glib-lowlevel.h>
#include <QDeclarativeContext>
#include <QDBusInterface>
#include <QDBusPendingCall>
#include <QStringList>


static const gchar   CUACFM_STREAM_URL[] = "http://cien30.udc.es/cuacfm.mp3";
static const QString STORE_DBUS_IFACE("com.nokia.OviStoreClient");


static void setProperty(void* object,
                        const gchar* name,
                        const gchar* newValue)
{
    GValue value;
    memset(&value, 0x00, sizeof(GValue));
    g_value_init(&value, G_TYPE_STRING);
    g_value_set_string(&value, newValue);
    g_object_set_property(G_OBJECT(object), name, &value);
    g_value_unset(&value);
}


static void setProperty(void* object,
                        const gchar* name,
                        gboolean newValue)
{
    GValue value;
    memset(&value, 0x00, sizeof(GValue));
    g_value_init(&value, G_TYPE_BOOLEAN);
    g_value_set_boolean(&value, newValue);
    g_object_set_property(G_OBJECT(object), name, &value);
    g_value_unset(&value);
}


template<typename T> T
getProperty(void* object, const gchar* name)
{
    T::this_is_unimplemented;
}

template<> bool
getProperty<bool>(void* object, const gchar* name)
{
    GValue value;
    memset(&value, 0x00, sizeof(GValue));
    g_value_init(&value, G_TYPE_BOOLEAN);
    g_object_get_property(G_OBJECT(object), name, &value);
    gboolean returnValue = g_value_get_boolean(&value);
    g_value_reset(&value);
    return returnValue;
}


CFMController::CFMController(QDeclarativeContext* context):
    _context(context),
    _connection(NULL),
    _playbin(NULL),
    _statusText(""),
    _bufferFillRate(-1),
    _playPending(false),
    _connected(false)
{
    Q_ASSERT(context);

    DBusConnection *systembus = dbus_bus_get(DBUS_BUS_SYSTEM, NULL);
    if (!systembus) {
        qFatal("Could not connect to the D-Bus system bus");
    }
    dbus_connection_setup_with_g_main(systembus, NULL);

    if ((_connection = con_ic_connection_new ()) == NULL) {
        qFatal("Coult not create ConIcConnection object");
    }

    g_signal_connect(G_OBJECT(_connection), "connection-event",
                     G_CALLBACK(CFMController::handleConnectionEvent), this);
    ::setProperty(_connection, "automatic-connection-events", true);

    // Register the controller in the QML context.
    _context->setContextProperty("controller", this);
}


GstElement* CFMController::gstPipeline()
{
    if (!_playbin) {
        if ((_playbin = gst_element_factory_make("playbin2", "playbin")) == NULL) {
            qFatal("Could not create GStreamer playbin2 element");
        }
        g_signal_connect(gst_pipeline_get_bus(GST_PIPELINE(_playbin)), "message",
                         G_CALLBACK(CFMController::handleGstMessage), this);
        ::setProperty(_playbin, "uri", CUACFM_STREAM_URL);
        qDebug("gstreamer: created playbin2 element");
    }
    return _playbin;
}


void CFMController::destroyGstPipeline()
{
    if (_playbin) {
        gst_element_set_state(_playbin, GST_STATE_PAUSED);
        gst_object_unref(_playbin);
        _bufferFillRate = -1;
        _playbin = NULL;
        qDebug("gstreamer: destroyed playbin2 element");
    }
}


CFMController::~CFMController()
{
    destroyGstPipeline();
    g_object_unref(_connection);
}


bool CFMController::isMuted() const
{
    return _playbin ? ::getProperty<bool>(_playbin, "mute") : false;
}


void CFMController::setMuted(bool muteValue)
{
    if (_playbin) {
        ::setProperty(_playbin, "mute", muteValue);
        emit mutedStatusChanged(muteValue);
        updateStatusText();
    }
}


bool CFMController::isPlaying() const
{
    if (!_playbin)
        return false;

    if (_playPending)
        return false;

    GstState state;
    GstState nextState;

    gst_element_get_state(_playbin,
                          &state,
                          &nextState,
                          GST_CLOCK_TIME_NONE);

    /*
     * If the element is pending to transition to a different state,
     * use the next state as reference, because GStreamer will be
     * changing it behind our back to that.
     */
    if (nextState != GST_STATE_VOID_PENDING)
        state = nextState;

    switch (state) {
        case GST_STATE_NULL:  /* Initial state (of course, not playing) */
        case GST_STATE_READY: /* Ready to go to pause, but not playing either. */
        case GST_STATE_PAUSED:
            return false;

        case GST_STATE_PLAYING:
            return true;

        default:
            qCritical("Unrecognized GStreamer pipeline state!\n"
                      "CMFController::isPlaying() returning false anyway");
            return false;
    }
}


void CFMController::setPlaying(bool playingValue)
{
    if (playingValue) {
        if (!isNetworkAvailable()) {
            if (!_playPending) {
                qDebug("playback: no network, requesting -> pending");
                _playPending = true;
                setNetworkAvailable(true);
            }
            else {
                qDebug("playback: no network, still pending...");
            }
        }
        else {
            qDebug("playback: network available -> playing");
            _playPending = false;
            gst_element_set_state(gstPipeline(), GST_STATE_PLAYING);
            emit playingStatusChanged(true);
        }
    }
    else {
        qDebug("playback: stopped");
        _playPending = false;
        destroyGstPipeline();
        emit playingStatusChanged(false);
    }
    updateStatusText();
}


bool CFMController::isBuffering() const
{
    return _playbin && _bufferFillRate >= 0 && _bufferFillRate < 100;
}


int CFMController::getBufferingStatus() const
{
    return _bufferFillRate;
}


void CFMController::handleGstMessage(GstBus     *bus,
                                     GstMessage *message,
                                     gpointer    controllerptr)
{
    CFMController *controller = (CFMController*) controllerptr;

    Q_ASSERT(controller);
    Q_ASSERT(controller->_playbin);
    Q_UNUSED(bus);

    switch (GST_MESSAGE_TYPE(message)) {
        /*
         * Update the buffering status and handle pausing the playback when
         * the fill rate of the buffer is below 100% -- recommended in docs.
         */
        case GST_MESSAGE_BUFFERING: {
            bool wasBuffering = controller->isBuffering();

            gst_message_parse_buffering(message, &controller->_bufferFillRate);

            controller->emit bufferingStatusChanged(controller->_bufferFillRate);
            if (wasBuffering != controller->isBuffering())
                controller->emit bufferingChanged(controller->isBuffering());

            controller->updateStatusText();

            qDebug("Buffering: %u%%", controller->_bufferFillRate);
            if (controller->_bufferFillRate < 100 && controller->isPlaying())
                controller->setPlaying(false);
            else if (!controller->isPlaying())
                controller->setPlaying(true);
            break;
        }

        /*
         * Theorically the stream is infinite, still EOS may be received
         * e.g. if the server closes the connection. Handle it by pausing
         * the playback.
         */
        case GST_MESSAGE_EOS:
            controller->setPlaying(false);
            break;

        /*
         * Errors are both printed printed to stderr and signaled.
         * Warnings are printed out to stderr using qWarning().
         */
        case GST_MESSAGE_WARNING:
        case GST_MESSAGE_ERROR: {
            gchar *debuginfo = NULL;
            GError *error = NULL;

            // Note that playback must be stopped on errors.
            if (GST_MESSAGE_TYPE(message) == GST_MESSAGE_ERROR) {
                gst_message_parse_error(message, &error, &debuginfo);
                controller->setPlaying(false);
            }
            else {
                gst_message_parse_warning(message, &error, &debuginfo);
            }

            qWarning("GStreamer: %s", error->message);
            qDebug  ("  - debug: %s", debuginfo);

            if (GST_MESSAGE_TYPE(message) == GST_MESSAGE_ERROR)
                controller->emit playbackError(QString(error->message));

            g_error_free(error);
            g_free(debuginfo);
            break;
        }

        default: /* For the rest of messages, do nothing. */
            break;
    }
}


bool CFMController::isNetworkAvailable() const
{
    return _connected;
}


void CFMController::setNetworkAvailable(bool availability)
{
    if (!_connected && availability) {
        qDebug("network: connection requested");
        if (!con_ic_connection_connect(_connection, CON_IC_CONNECT_FLAG_NONE))
            qFatal("Failed sending D-Bus message to connection manager");
    }
}


void CFMController::handleConnectionEvent(ConIcConnection      *connection,
                                          ConIcConnectionEvent *event,
                                          gpointer              controllerptr)
{
    CFMController *controller = (CFMController*) controllerptr;

    Q_ASSERT(controller);
    Q_UNUSED(connection);

    ConIcConnectionStatus status = con_ic_connection_event_get_status(event);

    switch (status) {
        case CON_IC_STATUS_CONNECTED:
            qDebug("network: connected");
            // If playing was requested and we were waiting for a connection
            // event, start playing now.
            controller->emit networkAvailableChanged((controller->_connected = true));
            if (controller->_playPending) {
                controller->setPlaying(true);
                controller->_playPending = false;
            }
            break;

        case CON_IC_STATUS_DISCONNECTING:
            qDebug("network: disconnecting...");
            // If playing was enabled, then we need to pause and tell the
            // user about the connection being lost.
            //
            // TODO: Show notification on connection lost.
            controller->emit networkAvailableChanged((controller->_connected = false));
            controller->setPlaying(false);
            break;

        case CON_IC_STATUS_NETWORK_UP:
            qDebug("network: connecting...");
            break;

        case CON_IC_STATUS_DISCONNECTED:
            qDebug("network: disconnected");
            break;
    }

    controller->updateStatusText();
}


void CFMController::updateStatusText()
{
    static const QString bufferingText(tr("Buffering... %1%"));
    static const QString mutedText(tr("%1 - Muted"));
    QString newText;

    if (_connected) {
        if (isBuffering())
            newText = bufferingText.arg(getBufferingStatus());
        else if (isPlaying())
            newText = tr("Playing");
        else
            newText = tr("Paused");
    }
    else {
        // No connection
        if (_playPending)
            newText = tr("Connecting...");
        else
            newText = tr("Disconnected");
    }

    if (isMuted())
        newText = mutedText.arg(newText);

    if (newText != _statusText)
        emit statusTextChanged((_statusText = newText));
}


const QString& CFMController::getStatusText() const
{
    return _statusText;
}


void CFMController::openStoreClient(const QString& url) const
{
    // Based on
    // https://gitorious.org/n9-apps-client/n9-apps-client/blobs/master/daemon/notificationhandler.cpp#line178

    QDBusInterface storeif(STORE_DBUS_IFACE, "/", STORE_DBUS_IFACE,
                           QDBusConnection::sessionBus());

    QStringList params;
    params << url;

    storeif.asyncCall("LaunchWithLink", QVariant::fromValue(params));
}
