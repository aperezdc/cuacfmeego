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

	Label {
		text: controller.statusText
		color: UIConstants.COLOR_INVERTED_FOREGROUND
		anchors {
			horizontalCenter: parent.horizontalCenter
			top: parent.top
			topMargin: UIConstants.DEFAULT_MARGIN * 3
		}
		platformStyle: LabelStyle {
			fontPixelSize: UIConstants.FONT_XLARGE
		}
	}

	Image {
		anchors.centerIn: parent
		source: 'qrc:/resources/default.png'
	}

	Menu {
		id: mainMenu
		MenuLayout {
			MenuItem {
				//: Menu option in main page to open radio station website
				text: qsTr('Open website')
				onClicked: Qt.openUrlExternally('http://cuacfm.org')
			}
			MenuItem {
				//: Menu option in main page to open the about page
				text: qsTr('About')
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
			//: Play/Pause button in the lower toolbar of the main page
			text: (controller.playing || controller.buffering) ?  qsTr('Pause') : qsTr('Play')
			anchors.horizontalCenter: parent.horizontalCenter;
			onClicked: controller.setPlaying(!controller.isPlaying())
		}
	}
}

