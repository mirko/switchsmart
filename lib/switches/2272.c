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

#include "2272.h"
#define WORD_SIZE 6

char* _2272_convert(char* code) {
    int i;
    for(i=0;code[i]!='\0';i++) {
        switch(code[i]) {
            case '0':
                memcpy(buf+i*WORD_SIZE, "100110", WORD_SIZE); // 0 = 100 110 = dip-switch is down
                break;
            case '1':
                memcpy(buf+i*WORD_SIZE, "100100", WORD_SIZE); // 1 = 100 100 = dip-switch is up
                break;
            default:
                printf("BUG!\n");
        }
    }
    return (char *)&buf;
}

struct packet _2272_ctrl_pkg(char* code) {
    char data[DATA_MAX];
    memcpy(data, _2272_convert(code), WORD_SIZE*10);
    data[WORD_SIZE*10] = '\0';
    struct packet pkg = {
        .duration = 500 - SPI_TRANSFER_TIME,
        .count = 10,
    };
    strcpy(pkg.data, data);
    return pkg;
}

struct packet switch_2272_on(char* code) {
    //TODO: check length of code
    struct packet pkg = _2272_ctrl_pkg(code);
    memcpy(pkg.data+10*WORD_SIZE, "100100100110100", 15); // on => 100 100 100 110 + 100 SYNC
    return pkg;
}

struct packet switch_2272_off(char* code) {
    //TODO: check length of code
    struct packet pkg = _2272_ctrl_pkg(code);
    memcpy(pkg.data+10*WORD_SIZE, "100110100100100", 15); // off => 100 110 100 100 + 100 SYNC
    return pkg;
}
