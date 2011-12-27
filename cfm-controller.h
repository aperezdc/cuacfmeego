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

public slots:
    bool isMuted() const;
    void setMuted(bool muteValue);

    bool isPlaying() const;
    void setPlaying(bool playValue);

private:
    QDeclarativeContext* _context;
    GstElement *_playbin;
};

#endif /* !__cfm_controller_h__ */

