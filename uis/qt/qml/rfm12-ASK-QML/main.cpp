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

#define NAME "rfm12-ASK-for-linux-QML"
#define VERSION "0.1"

bool initializeXmlRpc(xmlrpc_env *envP)
{
    // initialize xmlrpc and its error-handling environment
    xmlrpc_env_init(envP);
    xmlrpc_client_init2(envP, XMLRPC_CLIENT_NO_FLAGS, NAME, VERSION, NULL, 0);
    if (envP->fault_occurred)
        return false;

    return true;
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    xmlrpc_env env;
    //QList<PowerSocket> powerSocketList;
    //PowerSocketListModel
    ListModel model(new PowerSocket, qApp);
    QString serverURL = "localhost:31337/RPC2";
    QString methodName = "getPowerSocketArray";
    bool xmlrpcInitialized = false;

    // set up QML viewer
    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationLockLandscape);
    viewer.setMainQmlFile(QLatin1String("qml/rfm12-ASK-QML/main.qml"));
    viewer.rootContext()->setContextProperty("powerSocketModel", &model);
    viewer.show();

    xmlrpcInitialized = initializeXmlRpc(&env);

    try
    {
        refreshPowerSockets(&env, &model, serverURL, methodName);
    } catch(QString& ex)
    {
        std::cerr << "Exception: " << (const char *)(ex.toAscii()) << std::endl;
        //exit(1);
    }

    /*PowerSocket blub = powerSockets["SteckdoseB"];
    blub.setLabel("Hallo");
    std::cout << (const char *)(blub.getLabel().toAscii()) << std::endl;
    PowerSocket blah = powerSockets["SteckdoseB"];
    std::cout << (const char *)(blah.getLabel().toAscii()) << std::endl;*/

    return app.exec();
}
