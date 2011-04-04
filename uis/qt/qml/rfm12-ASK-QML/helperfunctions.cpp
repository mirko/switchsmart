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

#include "helperfunctions.h"

bool initializeXmlRpc(xmlrpc_env *envP, xmlrpc_client **clientPP)
{

    //xmlrpc_client_init2(envP, XMLRPC_CLIENT_NO_FLAGS, NAME, VERSION, NULL, 0);
    if (*clientPP != NULL)
    {
        xmlrpc_client_teardown_global_const();
        xmlrpc_client_destroy(*clientPP);
        *clientPP = NULL;
    }
    if (*clientPP == NULL)
    {
        xmlrpc_client_setup_global_const(envP);
        xmlrpc_client_create(envP, XMLRPC_CLIENT_NO_FLAGS, NAME, VERSION, NULL, 0, clientPP);
    }
    if (envP->fault_occurred)
        return false;

    return true;
}

void assertOk(xmlrpc_env *const envP) throw (QString)
{
    if (envP->fault_occurred)
    {
        std::cerr << "ERROR: " << envP->fault_string << std::endl;
        throw QString(envP->fault_string);
    }
}

QString getStringFromXmlRpcStruct(xmlrpc_env *const envP,
                                      QString memberName,
                                      xmlrpc_value *const xmlrpcStructP) throw (QString)
{
    const char *ret;
    xmlrpc_value *structMemberP;

    xmlrpc_struct_find_value(envP, xmlrpcStructP, memberName.toAscii(), &structMemberP);
    assertOk(envP);

    xmlrpc_read_string(envP, structMemberP, &ret);
    assertOk(envP);

    return QString(ret);
}

int getIntFromXmlRpcStruct(xmlrpc_env *const envP,
                           QString memberName,
                           xmlrpc_value *const xmlrpcStructP) throw (QString)
{
    int ret;
    xmlrpc_value *structMemberP;

    xmlrpc_struct_find_value(envP, xmlrpcStructP, memberName.toAscii(), &structMemberP);
    assertOk(envP);

    xmlrpc_read_int(envP, structMemberP, &ret);
    assertOk(envP);

    return ret;
}
