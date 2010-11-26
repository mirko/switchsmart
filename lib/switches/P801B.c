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

packet switch_P801B_on(char system_code, char unit_code) {
    printf("switch on\n");
    packet pkg = {
        .duration = 200,
        .count = 1,
        //.data = "1110 1110 1110 1110 1110 1110 1110 1110 1110 1110    1000 1110 1000 1110 1000 1000 1000 1110 1000 1110 1000    1110 1000 1000 1000", // C an
        .data = "1110111011101110111011101110111011101110100011101000111010001000100011101000111010001110100010001000",
    };
    return pkg;
}

packet switch_P801B_off(char system_code, char unit_code) {
    printf("switch off\n");
    packet pkg;
    return pkg;
}
