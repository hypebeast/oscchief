# oscchief

oscchief is a command line tool for sending and receiving OSC packets. Received
packets will be dump to stdout for further inspections.

oscchief is written in C and depends only on [liblo](http://liblo.sourceforge.net/).
It runs on Mac OS X and Linux.


## Installation

oscchief is written in portable C and runs on Mac OS X and Linux platforms.
Currently it's necessary to compile it from source. But this is easy and should
not be problematic.

In the future prebuild binaries for all supported platforms should be available.

### Instructions

First, download liblo from [here](http://liblo.sourceforge.net/) and unpack it.
After that the following steps installs liblo:

	$ cd liblo-x.xx
	$ ./configure --prefix=/usr/local
	$ make
	$ make install


Once liblo is installed you can compile and install oscchief:

	$ git clone https://github.com/hypebeast/oscchief.git oscchief
	$ cd oscchief
	$ make
	$ sudo make install


## Usage

For showing the help message execute oscchief with the '-h' option:

	$ oscchief -h

The following listing shows all available options:

	usage: oscchief send HOST PORT OSCADDRESS TYPES ARGUMENTS
           oscchief listen PORT

	positional arguments:
	    HOST: IP address of the host where you want to send your OSC message
	    PORT: Port number
	    OSCADDRESS: OSC address where you want to send your message
	    TYPES: OSC type tags. Supported types:
	        i - 32 Bit integer
	        h - 64 Bit integer
	        f - 32 Bit float
	        d - 64 Bit double
	        c - Char
	        s - String
	        T - True (no argument required)
	        F - False (no argument required)
	        N - Nil (no argument required)

	optional arguments:
	     -h - Shows this help message

There are two modes of operation:

* Client: send OSC packets
* Server: listen for incoming OSC packets and prints it to stdout

To use oscchief as an OSC client to send OSC packets use it as follows:

	$ oscchief send HOST PORT OSCADRESS TYPES ARGUMENTS

If you want to use oscchief as a OSC server use it with following options:

	$ oscchief listen PORT


### Examples

#### Client Usage

The following command sends two integer and one boolean argument:

	$ oscchief send 192.168.0.10 7028 /osc/address iTi 10 45

If you want to send only a OSC packet and no arguments:

	$ oscchief send 192.168.0.10 7028 /osc/address

### Server Usage

To listen on port 7028 for incoming OSC packets use the following command:

	$ oscchief listen 7028


## Credits

oscchief is based on the oscdump and oscsend tools from the liblo library written by
Kentaro Fukuchi.


## Licensing

oscchief is licensed under the terms of [The MIT License (MIT)](http://opensource.org/licenses/MIT).

	/*
	 * oscdump - Receive and dump OpenSound Control messages.
	 *
	 * Copyright (C) 2008 Kentaro Fukuchi <fukuchi@megaui.net>
	 *
	 * This program is free software; you can redistribute it and/or
	 * modify it under the terms of the GNU Lesser General Public License
	 * as published by the Free Software Foundation; either version 2.1 of
	 * the License, or (at your option) any later version.
	 *
	 * This program is distributed in the hope that it will be useful, but
	 * WITHOUT ANY WARRANTY; without even the implied warranty of
	 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	 * Lesser General Public License for more details.
	 *
	 * You should have received a copy of the GNU Lesser General Public
	 * License along with this program; if not, write to the Free Software
	 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
	 * 02110-1301 USA.
	 */