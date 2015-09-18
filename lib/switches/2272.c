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

#include <string.h>
#include <stdio.h>

#include "2272.h"

static int _2272_ctrl_pkg(struct packet *packet, const char* code, const char *suffix) {
    int i;
    memset(packet, 0, sizeof(*packet));
    packet->duration = 500 - SPI_TRANSFER_TIME;
    packet->count    = 10;

    for (i=0;i<6;++i)
        if ((code[i] & 0xfe) != '0' )
            return -1;
    if (code[7])
            return -1;

    snprintf(packet.data, DATA_MAX, "1001%c01001%c01001%c01001%c01001%c01001%c0%s",
        code[0]^1,code[1]^1,code[2]^1,code[3]^1,code[5]^1,code[6]^1, suffix);
    return 0;
}

struct packet switch_2272_on(char* code) {
    struct packet pkg;
    if (_2272_ctrl_pkg(&pkg, code, "100100100110100"))  // on => 100 100 100 110 + 100 SYNC
        printf("BUG!\n");
    return pkg;
}

struct packet switch_2272_off(char* code) {
    struct packet pkg;
    if (_2272_ctrl_pkg(&pkg, code, "100110100100100")) // off => 100 110 100 100 + 100 SYNC
        printf("BUG!\n");
    return pkg;
}
