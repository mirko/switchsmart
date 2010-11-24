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

all: mod_compile lib_compile app_compile

clean: mod_clean lib_clean app_clean

mod_compile:
	cd kernel && make

lib_compile:
	cd lib && make

app_compile: lib_compile
	cd app && make

mod_clean:
	cd kernel && make clean

lib_clean:
	cd lib && make clean

app_clean:
	cd app && make clean
