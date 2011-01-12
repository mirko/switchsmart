#include <xmlrpc-c/base.h>
#include <xmlrpc-c/server.h>
#include <xmlrpc-c/server_abyss.h>

#include "../lib/core.h"

xmlrpc_value * id;
xmlrpc_int ret;
struct device* ptr;
struct packet pkg;

int _lookup_device(xmlrpc_value* val) {
    ptr = lookup_device((char *)val);
    if (!ptr) {
        err("device lookup failed - requested device is not configured");
        ret = 0;
        return 0;
    }
    return 1;
}

static xmlrpc_value* on(
    xmlrpc_env *   const envP,
    xmlrpc_value * const paramArrayP,
    void *         const serverInfo,
    void *         const channelInfo
    ) {

    xmlrpc_decompose_value(envP, paramArrayP, "(s)", &id);
    if (envP->fault_occurred)
        return NULL;

    if(_lookup_device(id)) {
        pkg = (ptr->on)(ptr->code);
        ret = pkg_send(&pkg);
    }

    /* Return our result. */
    return xmlrpc_build_value(envP, "i", ret);
}

static xmlrpc_value* off(
    xmlrpc_env *   const envP,
    xmlrpc_value * const paramArrayP,
    void *         const serverInfo,
    void *         const channelInfo
    ) {

    xmlrpc_decompose_value(envP, paramArrayP, "(s)", &id);
    if (envP->fault_occurred)
        return NULL;

    if(_lookup_device(id)) {
        pkg = (ptr->off)(ptr->code);
        ret = pkg_send(&pkg);
    }

    /* Return our result. */
    return xmlrpc_build_value(envP, "i", ret);
}

int main(int const argc, const char ** const argv) {

    create_objs_by_cfg();

    struct xmlrpc_method_info3 const _on = {
        /* .methodName     = */ "on",
        /* .methodFunction = */ &on,
    };

    struct xmlrpc_method_info3 const _off = {
        /* .methodName     = */ "off",
        /* .methodFunction = */ &off,
    };

    xmlrpc_server_abyss_parms serverparm;
    xmlrpc_registry * registryP;
    xmlrpc_env env;

    if (argc-1 != 1) {
        fprintf(stderr, "You must specify 1 argument:  The TCP port "
                "number on which the server will accept connections "
                "for RPCs (8080 is a common choice).  "
                "You specified %d arguments.\n",  argc-1);
        exit(1);
    }

    xmlrpc_env_init(&env);

    registryP = xmlrpc_registry_new(&env);

    xmlrpc_registry_add_method3(&env, registryP, &_on);
    xmlrpc_registry_add_method3(&env, registryP, &_off);

    serverparm.config_file_name = NULL;
    serverparm.registryP        = registryP;
    serverparm.port_number      = atoi(argv[1]);

    printf("Running XML-RPC server...\n");

    xmlrpc_server_abyss(&env, &serverparm, XMLRPC_APSIZE(registryP));

    /* xmlrpc_server_abyss() never returns */

    return 0;
}
