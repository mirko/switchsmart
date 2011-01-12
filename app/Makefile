#
# rfm12 ASK driver - this driver module adds rfm12 ASK support to the
#                    linux kernel
#
# Copyright (C) 2010 Mirko Vogt <dev@nanl.de>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#

CFLAGS += -L../lib
LDFLAGS += -lrfm12

all:
	$(CC) $(CFLAGS) $(LDFLAGS) rfm12_2272_A_on.c -o rfm12_2272_A_on
	$(CC) $(CFLAGS) $(LDFLAGS) rfm12_2272_A_off.c -o rfm12_2272_A_off

	$(CC) $(CFLAGS) $(LDFLAGS) rfm12_P801B_A_on.c -o rfm12_P801B_A_on
	$(CC) $(CFLAGS) $(LDFLAGS) rfm12_P801B_A_off.c -o rfm12_P801B_A_off

	$(CC) $(CFLAGS) $(LDFLAGS) rfm12_initialize_from_config.c -o rfm12_initialize_from_config

clean:
	rm -f rfm12_2272_A_on rfm12_2272_A_off
	rm -f rfm12_P801B_A_on rfm12_P801B_A_off
	rm -f rfm12_initialize_from_config
