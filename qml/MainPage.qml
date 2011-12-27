/*
 * MainPage.qml
 * Copyright (C) 2011 Adrian Perez <aperez@igalia.com>
 *
 * Distributed under terms of the GPLv3 license.
 */

import QtQuick 1.1
import com.nokia.meego 1.0


Page {
	id: window

	Component {
		id: aboutPage
		AboutPage { }
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
	}
}

