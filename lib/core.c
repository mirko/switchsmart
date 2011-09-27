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

#include <errno.h>

#define CONFIG_PATH "/etc/rfm12.cfg"
//#define CONFIG_PATH_ALTERNATE ""

char err_msg[512];
dictionary* cfg;

char mode;
int shm_id;
void* shm_ptr;

void fatal(char* msg) {
    fprintf(stderr, "ERROR: %s\n -> EXITING...\n", msg);
    exit(23);
}

void err(char* msg) {
    fprintf(stderr, "ERROR: %s\n", msg);
}

int get_count_configured_objs() {
    // only if there's no shm segment yet - otherwise count existing array  // HAHA, how to count existing array?!
    cfg = iniparser_load(CONFIG_PATH);
    if (!cfg) {
        sprintf(err_msg, "can not access config file %s", CONFIG_PATH);
        fatal(err_msg);
    }
    return iniparser_getnsec(cfg);
}

#ifdef _USE_SHM
int is_shm_already_allocated() {
    if((shmget(SHM_KEY, NULL, (SHM_MODE | IPC_EXCL))) < 0) {
        if (errno == ENOENT) {
            // there is no shared memory segment identified by <SHM_KEY> yet
            printf("no shared memory yet\n");
            return 0;
        }
        else {
            // proper error handling
            fatal("something went wrong while checking for shared memory segment");
        }
    }
    else {
        printf("shared memory already allocated\n");
        return 1;
    }
    return -EINVAL;
}
#endif

void create_objs_by_cfg(sections_count) {
    if (!cfg) {
        sprintf(err_msg, "can not access config file %s", CONFIG_PATH);
        fatal(err_msg);
    }

    int i = 0;

    char _buf[ASCIILINESZ*2+1];

    char* section;
    char* label;
    char* code;
    char* product;
    char* category;

    int section__len;

    // the iniparser does not allow us to iterate over sections but just calling them by name -
    // workarounding that is hacky - however actually I don't care since it keeps the bins small
    // and is called just once at startup
    for (;i<sections_count;i++) {
        section = iniparser_getsecname(cfg, i);
        printf("reading section: %s\n", section);

        //dev_arr[i].id = section;

        //strncpy(dev_arr[i].id, section, CONFIG_STRING_MAX_LENGTH);
        //dev_arr[i].id[CONFIG_STRING_MAX_LENGTH-1]= '\0';
        dev_arr[i].id = i;

        section__len = strlen(section);

        strcpy(_buf, section);
        _buf[section__len] = ':';

        strcpy(_buf+section__len+1, "label");
        label   = iniparser_getstring(cfg, _buf, NULL);
        if(!label)
            fatal("<label> is not set in config file");

        strcpy(_buf+section__len+1, "code");
        code    = iniparser_getstring(cfg, _buf, NULL);
        if(!code)
            fatal("<code> is not set in config file");

        strcpy(_buf+section__len+1, "product");
        product = iniparser_getstring(cfg, _buf, NULL);
        if(!product)
            fatal("<product> is not set in config file");

        strcpy(_buf+section__len+1, "category");
        category = iniparser_getstring(cfg, _buf, NULL);
        if(!category)
            fatal("<category> is not set in config file");

        strncpy(dev_arr[i].label, label, CONFIG_STRING_MAX_LENGTH);
        dev_arr[i].label[CONFIG_STRING_MAX_LENGTH-1]= '\0';
        strncpy(dev_arr[i].code, code, CONFIG_STRING_MAX_LENGTH);
        dev_arr[i].code[CONFIG_STRING_MAX_LENGTH-1]= '\0';
        strncpy(dev_arr[i].product, product, CONFIG_STRING_MAX_LENGTH);
        dev_arr[i].product[CONFIG_STRING_MAX_LENGTH-1]= '\0';
        strncpy(dev_arr[i].category, category, CONFIG_STRING_MAX_LENGTH);
        dev_arr[i].category[CONFIG_STRING_MAX_LENGTH-1]= '\0';

        //TODO: free allocated space by iniparser (ptrs as label, code, product, section point to)

        //dev_arr[i].label = label;
        //dev_arr[i].code = code;
        //dev_arr[i].product = product;

        printf("created object:\n");
        printf("  id:      %i\n", dev_arr[i].id);
        printf("  label:   %s\n", dev_arr[i].label);
        printf("  product: %s\n", dev_arr[i].product);
        printf("  category: %s\n", dev_arr[i].category);
        printf("  code:    %s\n", dev_arr[i].code);

//        printf("put into dict: %s\n", dev_arr[i].id);
//        dictionary_set(dev_dict, dev_arr[i].id, (char *)&dev_arr[i], 0);
//        printf("device: %d\n", i);
//        printf("Label: %s\n", (((struct device*)dictionary_get(dev_dict, dev_arr[0].id, NULL))->label));
    }

    // // sections count should represent the count of confnigured devices from now on
    // if iniparser_find_entry("general")
    //     sections_count--;
    //

    //return sections_count;
}


int init() {
#ifdef _USE_SHM
    #pragma message("using shared memory")
    int dev_arr_cnt;

    if(is_shm_already_allocated()) {
        mode = 'c';

        if((shm_id = shmget(SHM_KEY, sizeof(int), SHM_MODE)) < 0)
            fatal("can't get shared memory segment");
        if((shm_ptr = shmat(shm_id, NULL, 0)) == (void *)-1) // why?!
            fatal("can't attach to shared memory segment");

        memcpy(&dev_arr_cnt, shm_ptr, sizeof(int));

        shmdt(shm_ptr);

        printf("Allocated devices: %d\n", dev_arr_cnt);

        if((shm_id = shmget(SHM_KEY, sizeof(int)+dev_arr_cnt*sizeof(struct device), SHM_MODE)) < 0)
            fatal("can't get shared memory segment");
        if((shm_ptr = shmat(shm_id, NULL, 0)) == (void *)-1) // why?!
            fatal("can't attach to shared memory segment");
        
        dev_arr = shm_ptr + sizeof(int);

        printf("successfully attached to shared memory\n");
    }
    else {
        mode = 's';

        // no server instance running yet...
        dev_arr_cnt = get_count_configured_objs();
        printf("read %d objects (configured devices) into memory\n", dev_arr_cnt);
        printf("allocating %lo bytes of shared memory\n", sizeof(int)+dev_arr_cnt*sizeof(struct device));

        //if((shm_id = shmget(SHM_KEY, sizeof(int) + dev_arr_cnt*sizeof(struct device) + sizeof(dictionary)+dev_arr_cnt*sizeof(char*)+dev_arr_cnt*sizeof(char*)+dev_arr_cnt*sizeof(unsigned), IPC_CREAT | SHM_MODE)) < 0)
        if((shm_id = shmget(SHM_KEY, sizeof(int) + dev_arr_cnt*sizeof(struct device), IPC_CREAT | SHM_MODE)) < 0)
            fatal("can't create shared memory segment");
        if((shm_ptr = shmat(shm_id, NULL, 0)) == (void *)-1) // why?!
            fatal("can't attach to shared memory segment");

        memcpy(shm_ptr, &dev_arr_cnt, sizeof(int));
        
        dev_arr = shm_ptr + sizeof(int);

        create_objs_by_cfg(dev_arr_cnt);
    }
    return dev_arr_cnt;
#else
    // do usual malloc stuff here
#endif
}

#ifdef _USE_SHM
void dt_shm() {
    shmdt(shm_ptr);
    if (mode == 's')
        shmctl(shm_id, IPC_RMID, NULL);
}
#endif

void* str_to_func_ptr(char* str, char func) {
        if(!strcmp(str, "P801B")) {
            if(func)
                return &switch_P801B_on;
            else
                return &switch_P801B_off;
        }
        if(!strcmp(str, "2272")) {
            if(func)
                return &switch_2272_on;
            else
                return &switch_2272_off;
        }
        fatal("<product> set for this device does not exist");
        return NULL;
}

int control(struct device* dev, int value) {
    struct packet (*ptr)();
    struct packet pkg;

    ptr = str_to_func_ptr(dev->product, value);
    pkg = (ptr)(dev->code);

    if (pkg_send(&pkg)) {
        dev->state = value;
        return 1;
    }
    return 0;
}

//struct device* lookup_device(char* id) {
struct device* lookup_device(int id) {
    //if (!dev_dict)
    if (!dev_arr)
        fatal("configuration not initialized but accessed");
    return (&dev_arr[id]);
}

int pkg_send(struct packet *_packet) {
    FILE *fd = fopen(DEVICE_NAME, "w");
    size_t res;

    //printf("duration: %u\n", _packet->duration);
    //printf("count: %u\n", _packet->count);
    //printf("data: %s\n", _packet->data);
    //printf("sizeof: %lu\n", sizeof(_packet));

    if (fd == NULL) {
        printf("can not open device: %s\n", DEVICE_NAME);
        return EXIT_FAILURE;
    }
    if ((res = fwrite(_packet, 1, sizeof(*_packet), fd)) != (sizeof(*_packet))) {
        printf("couldn't write full packet of %lu Bytes but only %lu Bytes\n", sizeof(*_packet), res);
        return EXIT_FAILURE;
    }
    fclose(fd);
    return 1;
}
