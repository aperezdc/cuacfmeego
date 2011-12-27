/*
 * main.qml
 * Copyright (C) 2011 Adrian Perez <aperez@igalia.com>
 *
 * Distributed under terms of the GPLv3 license.
 */


import QtQuick 1.1
import com.nokia.meego 1.0

PageStackWindow {
	id: appWindow

	Component.onCompleted: theme.inverted = true

	initialPage: mainPage
	showStatusBar: appWindow.inPortrait

	MainPage {
		id: mainPage
		orientationLock: PageOrientation.LockPortrait
	}

	platformStyle: PageStackWindowStyle {
		background: 'image://theme/meegotouch-video-background'
		backgroundFillMode: Image.Stretch
	}
}

