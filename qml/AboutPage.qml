/*
 * about.qml
 * Copyright (C) 2011 Simon Pena <aperez@igalia.com>
 * Copyright (C) 2011-2012 Adrian Perez <aperez@igalia.com>
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

		Component.onCompleted: {
				aboutOptions.get(0).title = qsTr('Recommend this application')
				aboutOptions.get(1).title = qsTr('Tell us what you think')
				aboutOptions.get(2).title = qsTr('Rate us in the Nokia Store')
				aboutOptions.get(3).title = qsTr('Follow us on Twitter')
				aboutOptions.get(4).title = qsTr('Check our other applications')
		}

		ListModel {
				id: aboutOptions
				ListElement {
						title: 'Recommend this application'
						action: 'openExternally'
						data: 'mailto:?subject=Download%20Cuac%20FM&body=Available%20at%20http://store.ovi.com/content/273753'
				}
				ListElement {
						title: 'Tell us what you think'
						action: 'openExternally'
						data: 'mailto:aperez@igalia.com?subject=Cuac%20FM'
				}
				ListElement {
						title: 'Rate us in the Nokia Store'
						action: 'openStore'
						data: 'http://store.ovi.com/content/273753'
				}
				ListElement {
						title: 'Follow us on Twitter'
						action: 'openExternally'
						data: 'http://twitter.com/aperezdc'
				}
				ListElement {
						title: 'Check our other applications'
						action: 'openStore'
						data: 'http://store.nokia.com/publisher/Igalia'
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
            bottomMargin: UIConstants.DEFAULT_MARGIN
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

            Label {
							id: aboutVersion
							text: 'Cuac FM 0.1.3'
							width: parent.width
							horizontalAlignment: Text.AlignHCenter
							color: UIConstants.COLOR_INVERTED_FOREGROUND
							platformStyle: LabelStyle {
								fontPixelSize: UIConstants.FONT_XLARGE
							}
            }

            Label {
							id: aboutCopyright
							text: 'Copyright © 2012 Adrian Perez'
							width: parent.width
							horizontalAlignment: Text.AlignHCenter
							color: UIConstants.COLOR_INVERTED_FOREGROUND
							platformStyle: LabelStyle {
								fontPixelSize: UIConstants.FONT_LSMALL
								fontFamily: UIConstants.FONT_FAMILY_LIGHT
							}
            }

						Column {
							height: repeater.model.count * UIConstants.LIST_ITEM_HEIGHT_SMALL
							width: parent.width
							anchors {
								topMargin: UIConstants.DEFAULT_MARGIN * 2
								bottomMargin: UIConstants.DEFAULT_MARGIN * 2
							}

							Repeater {
								id: repeater
								model: aboutOptions
								Item {
									height: UIConstants.LIST_ITEM_HEIGHT_SMALL
									width: parent.parent.width
									BorderImage {
										anchors.fill: parent
										visible: mouseArea.pressed
										source: 'image://theme/meegotouch-list-fullwidth-inverted-background-pressed-vertical-center'
									}
									Rectangle {
										anchors.bottom: parent.top
										width: parent.width
										height: 1
										color: '#444'
									}
									Label {
										text: model.title
										anchors {
											left: parent.left
											leftMargin: UIConstants.DEFAULT_MARGIN
											verticalCenter: parent.verticalCenter
										}
										platformStyle: LabelStyle {
											fontPixelSize: UIConstants.FONT_SLARGE
										}
										color: UIConstants.COLOR_INVERTED_FOREGROUND
									}
									Image {
										source: 'image://theme/icon-m-common-drilldown-arrow-inverse'
										anchors {
											right: parent.right
											rightMargin: UIConstants.DEFAULT_MARGIN
											verticalCenter: parent.verticalCenter
										}
									}
									Rectangle {
										anchors.bottom: parent.bottom
										width: parent.width
										height: 1
										color: '#444'
										visible: index === repeater.model.count - 1
									}
									MouseArea {
										id: mouseArea
										anchors.fill: parent
										onClicked: {
											if (model.action === 'openStore') {
												controller.openStoreClient(model.data)
											} else if (model.action === 'openExternally') {
												Qt.openUrlExternally(model.data)
											}
										}
									}
								}
							}
						}

            Text {
                id: aboutContentDisclaimer
                font.family: UIConstants.FONT_FAMILY
                font.pixelSize: UIConstants.FONT_LSMALL
                color: UIConstants.COLOR_INVERTED_FOREGROUND
                width: parent.width
                wrapMode: Text.WordWrap
                //: Disclaimer in about page
                text: qsTr('This application displays content from <a href="http://cuacfm.org/">cuacfm.org</a> but is not endorsed nor certified by Cuac FM.')
                onLinkActivated: Qt.openUrlExternally(link)
            }

        		Button {
        			anchors.horizontalCenter: parent.horizontalCenter
        			//: Button which open the license text popup from the about page
        			text: qsTr('Licensing terms')
        			onClicked: licenseDialog.open()
        		}
        }
    }

		QueryDialog {
			id: licenseDialog
			message: license
			//: Button which closes the license popup in the about page
			acceptButtonText: qsTr('Dismiss')
		}

    ScrollDecorator {
        flickableItem: flick
        anchors.rightMargin: -UIConstants.DEFAULT_MARGIN
    }
}
