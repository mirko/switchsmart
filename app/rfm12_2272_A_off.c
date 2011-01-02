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

#include "../lib/core.h"

int main(void)
{
    //struct device* dev_arr;
    //dev_arr = malloc(sizeof(device) * 2);
    //if(!dev_arr)
    //    return NULL;
    struct device dev_arr[2];
    struct device tmp;

    //tmp.type = SOCKET_TYPE_2272;
    tmp.type = 0;
    tmp.id = "ROOM_1__LAMP_1";
    tmp.category = "living room";
    tmp.label = "ceiling light";
    tmp.on = &switch_2272_on;
    tmp.off = &switch_2272_off;
    //dev_arr[0] = tmp;

    //tmp.type = SOCKET_TYPE_2272;
    //tmp.id = "ROOM_1__LAMP_2";
    //tmp.category = "living room";
    //tmp.name = "desktop";
    ////tmp.on_function_t = *2272_on,
    ////tmp.off_function_t = *2272_off,
    //dev_arr[1] = tmp;

//    printf("test: %p\n", dev_arr[0].on);
    //printf("test: %s\n", dev_arr[1].name);

    struct packet pkg;

    pkg = (tmp.off)("0000000100");
//                  sys  unit

    printf("count:    <%i>\n", pkg.count);
    printf("duration: <%i>\n", pkg.duration);
    printf("data:     <%i>\n", pkg.data);
    //printf("string: <%s>\n", str);

    return write(&pkg);
}
