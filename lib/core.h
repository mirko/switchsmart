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


#define DEVICE_NAME "/dev/rfm12_ask"

#define DATA_MAX 512

typedef struct packet {
    unsigned int duration;
    unsigned int count;
    char data[DATA_MAX]; // payload
} packet;

enum type {
    SOCKET_TYPE_2272,
};

typedef struct device {
    int type;
    char* id;
    char* label;
    char* category;
    packet (*on)(char system_code, char unit_code);
    packet (*off)(char system_code, char unit_code);
    int state;
} device;

#include "switches/2272.h"

#endif
