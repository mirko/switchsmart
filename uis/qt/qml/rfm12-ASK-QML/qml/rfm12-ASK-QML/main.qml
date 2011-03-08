//
// rfm12-ASK-for-linux QML GUI
//
// Copyright (C) 2011 Simon Lang <slangdev@googlemail.com>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

import Qt 4.7

Rectangle {
    id: mainScreen
    width: 800
    height: 600

    SystemPalette { id: activePalette }

    // PowerSocket list
    ScrollableList {
        id: powerSocketList
        property int margin: 5
        anchors {
            left: mainScreen.left; leftMargin: margin
            top: mainScreen.top; topMargin: margin
            bottom: settings.top//; bottomMargin: margin
            right: mainScreen.right; rightMargin: margin
        }
        //width: 400
        color: activePalette.alternateBase

        model: powerSocketModel

        delegate: Item {
            height: 50
            width: parent.width
            anchors.left: parent.left
            //anchors.leftMargin: 5

            Text {
                id: labelText
                font.pointSize: 14
                text: label
                color: activePalette.text
                anchors {
                    left: parent.left
                    leftMargin: 5
                    top: parent.top
                }
            }
            Text {
                id: idCodeText
                font.pointSize: 9
                text: id + " / " + code
                color: activePalette.text
                anchors {
                    left: parent.left
                    leftMargin: 5
                    top: labelText.bottom
                    topMargin: 2
                }
            }
            Text {
                id: stateText
                font.pointSize: 12
                text: socketState == 0 ? "Off" : "On"
                color: activePalette.text
                anchors {
                    right: parent.right
                    rightMargin: 5
                    verticalCenter: parent.verticalCenter
                }

            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    view.currentIndex = index;
                    powerSocketModel.control(id, socketState == 0 ? 1 : 0);
                }
            }
        }
    }

    // settings
    Rectangle {
        id: settings
        property int margin: 5
        anchors {
            left: mainScreen.left; leftMargin: margin
            //top: powerSocketList.bottom
            bottom: mainScreen.bottom//; bottomMargin: margin
            right: mainScreen.right; rightMargin: margin
        }
        height: 30
        color: activePalette.base

        Rectangle {
            id: textInputContainer
            property alias serverAddress: addressInput.text
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            border.width: 1
            height: addressInput.height + 4
            width: 300
            radius: 2
            smooth: true

            TextInput {
                id: addressInput
                text: "<server address>:<port>"
                property string previousText: "<server address>:<port>"
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 5
                anchors.rightMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                onAccepted: refreshButton.clicked
                onActiveFocusChanged: {
                    if (focus) {
                        previousText = text;
                        text = "";
                    }
                    else
                        text = previousText;
                }
            }
        }

        Button {
            id: refreshButton
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            text: "Refresh"
            onClicked: powerSocketModel.refresh(textInputContainer.serverAddress)
        }
    }

}
