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

CFLAGS += -Wall -D_USE_SHM

export CFLAGS

all: mod_compile lib_compile app_compile uis_compile

clean: mod_clean lib_clean app_clean uis_clean

mod_compile:
	$(MAKE) -C kernel

lib_compile:
	$(MAKE) -C lib

app_compile: lib_compile
	$(MAKE) -C app

uis_compile: lib_compile
	$(MAKE) -C uis

mod_clean:
	$(MAKE) clean -C kernel

lib_clean:
	$(MAKE) clean -C lib

app_clean:
	$(MAKE) clean -C app

uis_clean:
	$(MAKE) clean -C uis
