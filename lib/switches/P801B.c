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

#include "P801B.h"
#define WORD_SIZE 8

char* _p801b_convert_systemcode(char* code) {
    int i;
    for(i=0;code[i]!='\0';i++) {
        switch(code[i]) {
            case '0':
                memcpy(buf+i*WORD_SIZE, "10001110", WORD_SIZE); // 0 = 1000 1110 = dip-switch is down
                break;
            case '1':
                memcpy(buf+i*WORD_SIZE, "11101110", WORD_SIZE); // 1 = 1110 1110 = dip-switch is up
                break;
            default:
                printf("BUG!\n");
        }
    }
    return (char *)&buf;
}

char* _p801b_convert_unitcode(char* code) {
    int i;
    for(i=0;code[i]!='\0';i++) {
        switch(code[i]) {
            case '0':
                memcpy(buf+i*WORD_SIZE, "10001110", WORD_SIZE); // 0 = 1000 1110 = dip-switch is down
                break;
            case '1':
                memcpy(buf+i*WORD_SIZE, "10001000", WORD_SIZE); // 1 = 1000 1000 = dip-switch is up
                break;
            default:
                printf("BUG!\n");
        }
    }
    return (char *)&buf;
}

struct packet _p801b_ctrl_pkg(char* code) {
    printf("switch\n");
    char data[DATA_MAX];
    memcpy(data, _p801b_convert_systemcode(code), WORD_SIZE*10);
    memcpy(data+WORD_SIZE*5, _p801b_convert_unitcode(code+5), WORD_SIZE*5);
    data[WORD_SIZE*10] = '\0';
    struct packet pkg = {
        .duration = 200 - TRANS_DELAY,
        .count = 20,
    };
    strcpy(pkg.data, data);
    return pkg;
}

struct packet switch_P801B_on(char* code) {
    //TODO: check length of code
    printf("switch on\n");
    struct packet pkg = _p801b_ctrl_pkg(code);
    memcpy(pkg.data+10*WORD_SIZE, "10001110100010001000", 5*WORD_SIZE); // on => 1000 1110 1000 1000, sync => 1000
    return pkg;
}

struct packet switch_P801B_off(char* code) {
    //TODO: check length of code
    printf("switch off\n");
    struct packet pkg = _p801b_ctrl_pkg(code);
    memcpy(pkg.data+10*WORD_SIZE, "10001000100011101000", 5*WORD_SIZE); // off => 1000 1000 1000 1110, sync => 1000
    return pkg;
}
