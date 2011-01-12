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

#define DEVICE_NAME "/dev/rfm12"

// if changing, do not forget to change the associated within the kernel module as well
#define DATA_MAX 512

// define delay in microsends which is caused by the SPI transfer and therewith board specific - the actual specified TX duration is reduced by this value
#define TRANS_DELAY 200 // value for Netgear WGT634U - SPI via GPIOs / bitbanging

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
    char* id;
    char* label;
    char* category;
    int state; // MIN_INT = device (currently) not available
    char* code;
    struct packet (*on)(char* code);
    struct packet (*off)(char* code);
};

int send(struct packet _pkg)

#endif
