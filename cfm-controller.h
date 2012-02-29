/*
 * cfm-controller.h
 * Copyright (C) 2011 Adrian Perez <aperez@igalia.com>
 *
 * Distributed under terms of the GPLv3 license.
 */

#ifndef __cfm_controller_h__
#define __cfm_controller_h__

#include <QObject>
#include <QUrl>
#include <gst/gst.h>
#include <conic/conic.h>


class QDeclarativeContext;

class CFMController: public QObject
{
    Q_OBJECT

public:
    CFMController(QDeclarativeContext* context);
    virtual ~CFMController();

    Q_PROPERTY(int    bufferingStatus
               READ   getBufferingStatus
               NOTIFY bufferingStatusChanged)

    Q_PROPERTY(bool   buffering
               READ   isBuffering
               NOTIFY bufferingChanged)

    Q_PROPERTY(bool   playing
               READ   isPlaying
               WRITE  setPlaying
               NOTIFY playingStatusChanged)

    Q_PROPERTY(bool   muted
               READ   isMuted
               WRITE  setMuted
               NOTIFY mutedStatusChanged)

    Q_PROPERTY(bool   networkAvailable
               READ   isNetworkAvailable
               WRITE  setNetworkAvailable
               NOTIFY networkAvailableChanged)

public slots:
    bool isMuted() const;
    void setMuted(bool muteValue);

    bool isPlaying() const;
    void setPlaying(bool playValue);

    int  getBufferingStatus() const;
    bool isBuffering() const;

    bool isNetworkAvailable() const;
    void setNetworkAvailable(bool availability);

signals:
    void playbackError(const QString& message);
    void bufferingChanged(bool newStatus);
    void mutedStatusChanged(bool newStatus);
    void playingStatusChanged(bool newStatus);
    void bufferingStatusChanged(qreal fillRate);
    void networkAvailableChanged(bool newAvailability);

private:
    QDeclarativeContext* _context;
    ConIcConnection*     _connection;
    GstElement*          _playbin;
    gint                 _bufferFillRate;
    bool                 _playPending;
    bool                 _connected;

    static void handleGstMessage(GstBus*     bus,
                                 GstMessage* message,
                                 gpointer    controller);

    static void handleConnectionEvent(ConIcConnection*      connection,
                                      ConIcConnectionEvent* event,
                                      gpointer              controller);
};

#endif /* !__cfm_controller_h__ */

