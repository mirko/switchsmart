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

#include "core.h"

int write(packet *_packet) {
    printf("duration: %u\n", _packet->duration);
    printf("count: %u\n", _packet->count);
    printf("data: %s\n", _packet->data);
    printf("sizeof: %i\n", sizeof(_packet));
    FILE *fd = fopen(DEVICE_NAME, "w");
    if (fd == NULL) {
        printf("can not open device: %s\n", DEVICE_NAME);
        return EXIT_FAILURE;
    }
    if (fwrite(_packet, 1, sizeof(*_packet), fd) != sizeof(*_packet))
        return EXIT_FAILURE;
    fclose(fd);
    return 0;
}
