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

#include "2272.h"

struct packet switch_2272_on(char system_code, char unit_code) {
    printf("switch on\n");
    struct packet pkg = {
        .duration = 300,
        .count = 10, // 2 seems to be enough but let's get sure
        .data = "100100100100100100100100100100100100100110100110100110100110100100100110100",
    };
    return pkg;
}

struct packet switch_2272_off(char system_code, char unit_code) {
    printf("switch off\n");
    struct packet pkg = {
        .duration = 300,
        .count = 10, // 2 seems to be enough but let's get sure
        .data = "100100100100100100100100100100100100100110100110100110100110100110100100100",
    };
    return pkg;
}
