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

#define NAME "rfm12-ASK-for-linux-QML"
#define VERSION "0.1beta"

/**
  * Initializes XML-RPC using the given error environment.
  * The pointer to the newly created client will be stored at clientPP.
  */
bool initializeXmlRpc(xmlrpc_env *envP, xmlrpc_client **clientPP);

/**
  * If there was an XML-RPC error in envP, an exception is thrown, containing
  * the error message. If there was no error, the function will just return.
  */
void assertOk(xmlrpc_env *const envP) throw (QString);

/**
  * Returns the string stored in the given struct under the given member name.
  */
QString getStringFromXmlRpcStruct(xmlrpc_env *const envP,
                                  QString memberName,
                                  xmlrpc_value *const xmlrpcStructP) throw (QString);

/**
  * Returns the integer stored in the given struct under the given member name.
  */
int getIntFromXmlRpcStruct(xmlrpc_env *const envP,
                           QString memberName,
                           xmlrpc_value *const xmlrpcStructP) throw (QString);

#endif // HELPERFUNCTIONS_H
