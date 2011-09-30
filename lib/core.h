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

#ifndef _RFM12_H
#define _RFM12_H

#include <stdlib.h>
#include <stdio.h>

#include "switches/core.h"
//#include "iniparser/dictionary.h"

#define CONFIG_STRING_MAX_LENGTH 128

#define DEVICE_NAME "/dev/rfm12"

// if changing, do not forget to change the associated within the kernel module as well
#define DATA_MAX 512

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

#define SHM_KEY 234548
#define SHM_MODE 0666
#define SHM_SIZE 512


/* Since we're operating timing critical and need precision of hundreds of microseconds,
 * we can not just neglect the time an 16bit SPI transfer to the rfm12 module needs.
 * Because SPI might can be implemented in different ways (e.g. via GPIOs or native)
 * and used operational clock ix not fixed either, this constant is platform specific.
 * For now we're defining constants for some known and tested boards,
 * however measuring the time a transfer needs / calculating the time from e.g.
 * SPI_MAX_CLOCK would be much more elegant instead of having fixed values for
 * particular boards */

// define delay in microsends which is caused by SPI transfer
#ifndef SPI_TRANSFER_TIME
  #ifdef BRCM47XX
    #pragma message("using SPI_TRANSFER_TIME defined for BRCM47XX platform")
    #define SPI_TRANSFER_TIME 200 // value for Netgear WGT634U - SPI via GPIOs / bitbanging
  #elif INGENIC_JZ4740
    #pragma message("using SPI_TRANSFER_TIME defined for Ingenic JZ4740 platform")
    #define SPI_TRANSFER_TIME 100 // value for Qi LB60 / Ben NanoNote - SPI via GPIOs / bitbanging
  #else
    #pragma message("do not take SPI transfer time into account")
    #define SPI_TRANSFER_TIME 0 // do not take any delay caused by an SPI transfer into account
  #endif
#endif

char buf[DATA_MAX];

struct packet {
    unsigned int duration;
    unsigned int count;
    char data[DATA_MAX]; // payload
};

enum type {
    SOCKET_TYPE_2272,
};

struct device {
    int type;
    //char id[CONFIG_STRING_MAX_LENGTH];
    int id;
    char label[CONFIG_STRING_MAX_LENGTH];
    char category[CONFIG_STRING_MAX_LENGTH];
    int state; // MIN_INT = device (currently) not available
    char code[CONFIG_STRING_MAX_LENGTH];
    //struct packet (*on)(char* code); // deprecated
    //struct packet (*off)(char* code); // deprecated
    char product[CONFIG_STRING_MAX_LENGTH];
    int timeout; // in seconds
    int switched_on_for; // in seconds
};

struct device* dev_arr;
//dictionary* dev_dict;

//int create_objs_by_cfg(void);
void create_objs_by_cfg();
//struct device* lookup_device(char* id);
struct device* lookup_device(int id);
int pkg_send(struct packet *_packet);
void fatal(char* msg);
void err(char* msg);
int control(struct device* dev, int value);
int init();
void dt_shm();

#endif
