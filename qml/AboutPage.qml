/*
 * about.qml
 * Copyright (C) 2011 Simon Pena <aperez@igalia.com>
 * Copyright (C) 2011 Adrian Perez <aperez@igalia.com>
 *
 * Distributed under terms of the GPLv3 license.
 */

import QtQuick 1.1
import com.nokia.meego 1.0
import "file:///usr/lib/qt4/imports/com/meego/UIConstants.js" as UIConstants

Page {
    property string license: 'This program is free software: you can redistribute it and/or modify ' +
        'it under the terms of the GNU General Public License as published by ' +
        'the Free Software Foundation, either version 3 of the License, or ' +
        '(at your option) any later version.<br /><br />' +

        'This package is distributed in the hope that it will be useful, ' +
        'but WITHOUT ANY WARRANTY; without even the implied warranty of ' +
        'MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the ' +
        'GNU General Public License for more details.<br /><br />' +

        'You should have received a copy of the GNU General Public License ' +
        'along with this program. If not, see ' +
        '<a href="http://www.gnu.org/licenses">http://www.gnu.org/licenses</a><br /><br />'

    tools: ToolBarLayout {
        ToolIcon {
            iconId: 'toolbar-back'
            onClicked: pageStack.pop()
        }
    }

    Flickable {
        id: flick
        clip: true
        anchors.fill: parent
        anchors {
            topMargin: UIConstants.DEFAULT_MARGIN
            leftMargin: UIConstants.DEFAULT_MARGIN
            rightMargin: UIConstants.DEFAULT_MARGIN
        }
        contentHeight: contentColumn.height

        Column {
            id: contentColumn
            spacing: UIConstants.DEFAULT_MARGIN
            width: parent.width

            Image {
                id: aboutImage
                anchors.horizontalCenter: parent.horizontalCenter
                source: 'qrc:/cuacfmeego-logo.png'
            }

            Text {
                id: aboutVersion
                text: 'Cuac FM'
                anchors.horizontalCenter: parent.horizontalCenter
                font.family: UIConstants.FONT_FAMILY
                font.pixelSize: UIConstants.FONT_XLARGE
                color: !theme.inverted ?
                           UIConstants.COLOR_FOREGROUND :
                           UIConstants.COLOR_INVERTED_FOREGROUND
            }

            Text {
                id: aboutCopyright
                text: 'Copyright © 2011 Adrian Perez'
                anchors.horizontalCenter: parent.horizontalCenter
                font.family: UIConstants.FONT_FAMILY
                font.pixelSize: UIConstants.FONT_XLARGE
                color: !theme.inverted ?
                           UIConstants.COLOR_FOREGROUND :
                           UIConstants.COLOR_INVERTED_FOREGROUND
            }

            Text {
                id: aboutContact
                anchors.horizontalCenter: parent.horizontalCenter
                font.family: UIConstants.FONT_FAMILY
                font.pixelSize: UIConstants.FONT_LSMALL
                color: !theme.inverted ?
                           UIConstants.COLOR_FOREGROUND :
                           UIConstants.COLOR_INVERTED_FOREGROUND
                text: '<a href="mailto:aperez@igalia.com">aperez@igalia.com</a>'
                onLinkActivated: Qt.openUrlExternally(link)
            }

            Text {
                id: aboutMeneameDisclaimer
                font.family: UIConstants.FONT_FAMILY
                font.pixelSize: UIConstants.FONT_LSMALL
                color: !theme.inverted ?
                           UIConstants.COLOR_FOREGROUND :
                           UIConstants.COLOR_INVERTED_FOREGROUND
                width: parent.width
                wrapMode: Text.WordWrap
                text: 'This application displays content from <a href="http://cuacfm.org/">Last.fm</a> but is not endorsed nor certified by Cuac FM.'
                onLinkActivated: Qt.openUrlExternally(link)
            }

            Text {
                id: aboutLicense
                font.pixelSize: UIConstants.FONT_LSMALL
                color: !theme.inverted ?
                           UIConstants.COLOR_FOREGROUND :
                           UIConstants.COLOR_INVERTED_FOREGROUND
                width: parent.width
                wrapMode: Text.WordWrap
                font.family: "Nokia Pure Text Light"
                text: license
                onLinkActivated: Qt.openUrlExternally(link)
            }
        }
    }

    ScrollDecorator {
        flickableItem: flick
        anchors.rightMargin: -UIConstants.DEFAULT_MARGIN
    }
}
