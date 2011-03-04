#include "helperfunctions.h"

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

void refreshPowerSockets(xmlrpc_env *const envP,
                         ListModel *powerSocketsP,
                         QString serverURL,
                         QString methodName) throw (QString)
{
    powerSocketsP->clear();
    xmlrpc_value *powerSocketListP = xmlrpc_client_call(envP, serverURL.toAscii(), methodName.toAscii(),
                                                        "(i)", (xmlrpc_int32)5);
    assertOk(envP);

    int count = xmlrpc_array_size(envP, powerSocketListP);
    assertOk(envP);

    for (int i = 0; i < count; ++i)
    {
        xmlrpc_value *currentEntryP = xmlrpc_array_get_item(envP, powerSocketListP, i);
        QString id = getStringFromXmlRpcStruct(envP, "id", currentEntryP);
        QString label = getStringFromXmlRpcStruct(envP, "label", currentEntryP);
        QString code = getStringFromXmlRpcStruct(envP, "code", currentEntryP);
        int state = getIntFromXmlRpcStruct(envP, "state", currentEntryP);
        assertOk(envP);

        PowerSocket *powerSocketP = new PowerSocket(id, label, code, state, powerSocketsP);
        powerSocketsP->appendRow(powerSocketP);
    }

}
