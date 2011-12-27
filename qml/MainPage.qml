/*
 * MainPage.qml
 * Copyright (C) 2011 Adrian Perez <aperez@igalia.com>
 *
 * Distributed under terms of the GPLv3 license.
 */

import QtQuick 1.1
import com.nokia.meego 1.0
import "file:///usr/lib/qt4/imports/com/meego/UIConstants.js" as UIConstants


Page {
	id: window

	Component {
		id: aboutPage
		AboutPage { }
	}

	Text {
		color: theme.inverted ? UIConstants.COLOR_INVERTED_FOREGROUND : UIConstants.COLOR_FOREGROUND
		anchors.horizontalCenter: parent.horizontalCenter
		anchors.top: parent.top
		anchors.topMargin: UIConstants.DEFAULT_MARGIN * 3
		font.pixelSize: UIConstants.FONT_XLARGE
		font.family: UIConstants.FONT_FAMILY
		text: controller.buffering ? 'Buffering...'
		                           : (controller.playing ? 'Playing'
		                                                 : 'Paused')
	}

	Image {
		anchors.centerIn: parent
		source: 'qrc:/cuacfmeego-logo.png'
	}

	Menu {
		id: mainMenu
		MenuLayout {
			MenuItem {
				text: 'About'
				onClicked: appWindow.pageStack.push(aboutPage)
			}
		}
	}

	tools: ToolBarLayout {
		ToolIcon {
			iconId: 'toolbar-view-menu'
			anchors.right: parent.right
			onClicked: (mainMenu.status == DialogStatus.Closed) ?
								    mainMenu.open() : mainMenu.close()
		}

		ToolButton {
			id: playButton
			text: (controller.playing || controller.buffering) ? 'Pause' : 'Play'
			anchors.horizontalCenter: parent.horizontalCenter;
			onClicked: controller.setPlaying(!controller.isPlaying())
		}
	}
}

