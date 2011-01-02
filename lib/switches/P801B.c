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

#include "P801B.h"
#define WORD_SIZE 8

/* FACTS FOR SURE:
 *  ._ = short_long  = dip down = 1000 1110
 *  .. = short_short = dip up   = 1000 1000
 */

char* _p801b_convert(char* code) {
    int i, j;
    for(i=0;code[i]!='\0';i++) {
        //printf("<i> is: %d, code[%d]: %c\n", i, i, code[i]);
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
    buf[i*WORD_SIZE] = '\0';
    return &buf;
}

struct packet _p801b_ctrl_pkg(char* code) {
    printf("switch\n");
    char data[DATA_MAX];
    memcpy(data, _p801b_convert(code), WORD_SIZE*10);
    struct packet pkg = {
        .duration = 200,
        .count = 1,
    };
    strcpy(pkg.data, data);
    return pkg;
}

struct packet switch_P801B_on(char* code) {
    printf("switch on\n");
    struct packet pkg = _p801b_ctrl_pkg(code);
    //TODO: one "1000 1000" in the end might be missing
    memcpy(pkg.data+10*WORD_SIZE, "1000111010001000", 16); // on => 1000 1110 1000 1000
    return pkg;
}

struct packet switch_P801B_off(char* code) {
    printf("switch off\n");
    struct packet pkg = _p801b_ctrl_pkg(code);
    //TODO: one "1000 1000" in the end might be missing
    memcpy(pkg.data+10*WORD_SIZE, "1000100010001110", 16); // off => 1000 1000 1000 1110
    return pkg;
}
