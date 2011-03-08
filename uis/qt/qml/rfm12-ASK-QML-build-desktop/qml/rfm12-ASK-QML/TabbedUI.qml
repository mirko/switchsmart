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
    property int tabsWidth: 64
    property int tabIndex: 0

    anchors.fill: parent

    // contains the current tab view
    Rectangle {
        id: tabViewContainer
        height: parent.height

        anchors.right: parent.right
        anchors.left: tabBar.left
    }

    // the tab bar
    Rectangle {
        id: tabBar

        width: tabsWidth
        height: parent.height

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        //gradient?
    }
}
