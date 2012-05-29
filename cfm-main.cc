/*
 * cfm-main.cc
 * Copyright (C) 2011 Adrian Perez <aperez@igalia.com>
 *
 * Distributed under terms of the GPLv3 license.
 */

#include "cfm-controller.h"
#include <QLocale>
#include <QTextCodec>
#include <QTranslator>
#include <QApplication>
#include <QDeclarativeView>
#include <MDeclarativeCache>
#include <gst/gst.h>


static const QUrl INITIAL_VIEW("qrc:/qml/main.qml");


Q_DECL_EXPORT int
main (int argc, char *argv[])
{
    gst_init(&argc, &argv);

    QApplication *application(MDeclarativeCache::qApplication(argc, argv));

    application->setApplicationName("CuacFM");
    application->setOrganizationDomain("com.igalia.people");
    application->setOrganizationName("aperez");

    // Assume that strings in source fils are UTF-8 (as they should!)
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));

    QString locale(QLocale::system().name());
    QTranslator translator;

    if (translator.load("l10n/" + locale, ":/") || translator.load("l10n/en", ":/"))
        application->installTranslator(&translator);

    QDeclarativeView *view(MDeclarativeCache::qDeclarativeView());
    CFMController controller(view->rootContext());
    view->setSource(INITIAL_VIEW);
    view->showFullScreen();

    int result = application->exec();

    delete view;
    delete application;
    gst_deinit();

    return result;
}

