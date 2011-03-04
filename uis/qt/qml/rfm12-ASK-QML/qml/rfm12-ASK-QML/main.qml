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
            left: mainScreen.left; leftMargin: margin;
            top: mainScreen.top; topMargin: margin;
            bottom: mainScreen.bottom; bottomMargin: margin
        }
        width: 400

        model: powerSocketModel

        delegate: Item {
            height: 20
            width: parent.width
            anchors.left: parent.left
            anchors.leftMargin: 5

            Text {
                font.pixelSize: 14
                text: id + " / " + label + " / " + code + " / " + state
                anchors.left: parent.left
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    view.currentIndex = index;
                    /*view.model = index + 5;*/
                }
            }
        }
    }
}
