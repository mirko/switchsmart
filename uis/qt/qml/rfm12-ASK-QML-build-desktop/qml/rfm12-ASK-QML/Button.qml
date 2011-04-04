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

import QtQuick 1.0

Rectangle {
    id: button

    property alias text: text.text
    property alias font: text.font
    signal clicked

    SystemPalette { id: activePalette }

    height: text.height + 10
    width: text.width + 25
    border.width: 1
    radius: 3
    smooth: true

    color: mouseArea.pressed ? activePalette.dark : activePalette.button

    Text {
        id: text
        anchors.centerIn: parent
        font.pointSize: 9
        color: activePalette.buttonText
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: button.clicked()
    }
}
