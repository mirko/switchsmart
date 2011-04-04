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
    id: scrollableList
    property alias model: view.model
    property alias delegate: view.delegate
    property alias currentIndex: view.currentIndex
    property real itemHeight: 20

    SystemPalette { id: activePalette }

    clip: true

    ListView {
        id: view
        anchors.fill: parent

        //model: 200

        focus: true
        highlightMoveDuration: 1
        currentIndex: 0
        flickDeceleration: 600

        highlight: Rectangle { width: view.width; color: activePalette.highlight }
    }

    Keys.onDownPressed: view.incrementCurrentIndex()
    Keys.onUpPressed: view.decrementCurrentIndex()
}
