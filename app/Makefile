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
	$(CC) $(CFLAGS) $(LDFLAGS) rfm12_read_from_cfg_and_export_via_xmlrpc.c -o rfm12_read_from_cfg_and_export_via_xmlrpc $$(xmlrpc-c-config abyss-server --libs)

clean:
	rm -f rfm12_read_from_cfg_and_export_via_xmlrpc
