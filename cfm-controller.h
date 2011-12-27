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

public slots:
    bool isMuted() const;
    void setMuted(bool muteValue);

    bool isPlaying() const;
    void setPlaying(bool playValue);

    int  getBufferingStatus() const;
    bool isBuffering() const;

signals:
    void playbackError(const QString& message);
    void bufferingChanged(bool newStatus);
    void mutedStatusChanged(bool newStatus);
    void playingStatusChanged(bool newStatus);
    void bufferingStatusChanged(qreal fillRate);

private:
    QDeclarativeContext* _context;
    GstElement*          _playbin;
    gint                 _bufferFillRate;

    static void handleGstMessage(GstBus*     bus,
                                 GstMessage* message,
                                 gpointer    controller);
};

#endif /* !__cfm_controller_h__ */

