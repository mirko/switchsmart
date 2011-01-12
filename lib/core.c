//
// rfm12 ASK driver - this driver module adds rfm12 ASK support to the
//                    linux kernel
//
// Copyright (C) 2010 Mirko Vogt <dev@nanl.de>
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

#include "core.h"
#include "iniparser/iniparser.h"

#define CONFIG_PATH "/etc/rfm12.cfg"
//#define CONFIG_PATH_ALTERNATE ""

char err_msg[512];

void err(char* msg) {
    fprintf(stderr, "ERROR: %s\n -> EXITING...\n", msg);
    exit(23);
}

int create_objs_by_cfg(struct device* dev_arr) {
    dictionary* cfg = iniparser_load(CONFIG_PATH);
    if (!cfg) {
        sprintf(err_msg, "can not access config file %s", CONFIG_PATH);
        err(err_msg);
    }

    int i = 0;

    int sections_count = iniparser_getnsec(cfg);
    char _buf[ASCIILINESZ*2+1];

    char* section;
    char* label;
    char* code;
    char* product;

    int section__len;

    // the iniparser does not allow us to iterate over sections but just calling them by name -
    // workarounding that is hacky - however actually I don't care since it keeps the bins small
    // and is called just once at startup
    for (;i<sections_count;i++) {
        section = iniparser_getsecname(cfg, i);
        printf("reading section: %s\n", section);

        dev_arr[i].id = section;

        section__len = strlen(section);

        strcpy(_buf, section);
        _buf[section__len] = ':';

        strcpy(_buf+section__len+1, "label");
        label   = iniparser_getstring(cfg, _buf, NULL);
        if(!label)
            err("<label> is not set in config file");

        strcpy(_buf+section__len+1, "code");
        code    = iniparser_getstring(cfg, _buf, NULL);
        if(!code)
            err("<code> is not set in config file");

        strcpy(_buf+section__len+1, "product");
        product = iniparser_getstring(cfg, _buf, NULL);
        if(!product)
            err("<product> is not set in config file");

        dev_arr[i].label = label;
        dev_arr[i].code = code;

        // creating function pointers pointing to appropriate switch functions
        // would be using (s)scanf here a better solution?
        if(!strcmp(product, "P801B"))
            dev_arr[i].on = &switch_P801B_on;
            dev_arr[i].off = &switch_P801B_off;
        if(!strcmp(product, "2272"))
            dev_arr[i].on = &switch_2272_on;
            dev_arr[i].off = &switch_2272_off;

        printf("created object:\n");
        printf("  id:      %s\n", dev_arr[i].id);
        printf("  label:   %s\n", dev_arr[i].label);
        printf("  product: %s\n", product);
        printf("  code:    %s\n", dev_arr[i].code);
    }

    // // sections count should represent the count of confnigured devices from now on
    // if iniparser_find_entry("general")
    //     sections_count--;

    return 0;
}

int send(struct packet *_packet) {
    FILE *fd = fopen(DEVICE_NAME, "w");
    size_t res;

    printf("duration: %u\n", _packet->duration);
    printf("count: %u\n", _packet->count);
    printf("data: %s\n", _packet->data);
    printf("sizeof: %lu\n", sizeof(_packet));

    if (fd == NULL) {
        printf("can not open device: %s\n", DEVICE_NAME);
        return EXIT_FAILURE;
    }
    if (res = fwrite(_packet, 1, sizeof(*_packet), fd) != sizeof(*_packet))
        printf("couldn't write full packet of %lu Bytes but only %lu Bytes\n", sizeof(*_packet), res);
        return EXIT_FAILURE;
    fclose(fd);
    return 0;
}
