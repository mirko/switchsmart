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
#include <QDeclarativeContext>
#include <QDeclarativeItem>
#include <QObject>
#include <QStringListModel>
#include "qmlapplicationviewer.h"

#include <xmlrpc-c/base.h>
#include <xmlrpc-c/client.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "helperfunctions.h"
#include "powersocket.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    xmlrpc_env env; // error environment
    xmlrpc_client *clientP = NULL; // client used for RPCs
    ListModel model(new PowerSocket, &env, &clientP, qApp); // local list of powersockets

    bool xmlrpcInitialized = false;

    // set up QML viewer
    QmlApplicationViewer viewer;
    viewer.rootContext()->setContextProperty("powerSocketModel", &model);
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationLockLandscape);
    viewer.setMainQmlFile(QLatin1String("qml/rfm12-ASK-QML/main.qml"));
    viewer.show();

    // set up XML-RPC
    xmlrpc_env_init(&env);
    xmlrpcInitialized = initializeXmlRpc(&env, &clientP);

    return app.exec();
}
