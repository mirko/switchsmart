#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <xmlrpc-c/base.h>
#include <xmlrpc-c/client.h>
#include "qmlapplicationviewer.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "powersocket.h"

void assertOk(xmlrpc_env *const envP) throw (QString);

QString getStringFromXmlRpcStruct(xmlrpc_env *const envP,
                                  QString memberName,
                                  xmlrpc_value *const xmlrpcStructP) throw (QString);

int getIntFromXmlRpcStruct(xmlrpc_env *const envP,
                           QString memberName,
                           xmlrpc_value *const xmlrpcStructP) throw (QString);

void refreshPowerSockets(xmlrpc_env *const envP,
                         ListModel *powerSocketsP,
                         QString serverURL,
                         QString methodName) throw (QString);

#endif // HELPERFUNCTIONS_H
