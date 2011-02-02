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

#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include "qmlapplicationviewer.h"
//#include "../../../lib/core.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // set up QML viewer
    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::LockLandscape);
    viewer.setMainQmlFile(QLatin1String("qml/rfm12-ASK-QML/main.qml"));
    viewer.show();

    // QXmlRpc seems to be using deprecated methods of Qt, so we're using
    // ulxmlrpcpp(?) now
/*    xmlrpc::Client* client = new xmlrpc::Client(this);
    connect(client, SIGNAL(done(int, QVariant)), this, SLOT(processReturnValue(int, QVariant)));
    connect(client, SIGNAL(failed(int, int, QString)), this, SLOT(processFault(int, int, QString)));
    client->setHost("localhost", 80);*/

    // set up XML-RPC client
    //...

    return app.exec();
}
