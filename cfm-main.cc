/*
 * cfm-main.cc
 * Copyright (C) 2011 Adrian Perez <aperez@igalia.com>
 *
 * Distributed under terms of the GPLv3 license.
 */

#include "cfm-controller.h"
#include <QApplication>
#include <QDeclarativeView>
#include <MDeclarativeCache>


static const QUrl INITIAL_VIEW("qrc:/qml/main.qml");


Q_DECL_EXPORT int
main (int argc, char *argv[])
{
    QApplication *application(MDeclarativeCache::qApplication(argc, argv));
    QDeclarativeView    *view(MDeclarativeCache::qDeclarativeView());

    application->setApplicationName("CuacFM");

    CFMController controller(view->rootContext());
    view->setSource(INITIAL_VIEW);
    view->showFullScreen();

    int result = application->exec();

    delete view;
    delete application;

    return result;
}

