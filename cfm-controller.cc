/*
 * cfm-controller.cc
 * Copyright (C) 2011 Adrian Perez <aperez@igalia.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "cfm-controller.h"
#include <QDeclarativeContext>


static const gchar CUACFM_STREAM_URL[] = "http://cien30.udc.es/cuacfm.mp3";


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
    _context(context)
{
    Q_ASSERT(context);

    if ((_playbin = gst_element_factory_make("playbin2", "playbin")) == NULL) {
        qCritical("Could not create GStreamer playbin2 element");
    }

    ::setProperty(_playbin, "uri", CUACFM_STREAM_URL);

    // Register the controller in the QML context.
    _context->setContextProperty("controller", this);
}


CFMController::~CFMController()
{
    gst_object_unref(_playbin);
}


bool CFMController::isMuted() const
{
    return ::getProperty<bool>(_playbin, "mute");
}


void CFMController::setMuted(bool muteValue)
{
    ::setProperty(_playbin, "mute", muteValue);
}


bool CFMController::isPlaying() const
{
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
    gst_element_set_state(_playbin, playingValue ? GST_STATE_PLAYING
                                                 : GST_STATE_PAUSED);
}

