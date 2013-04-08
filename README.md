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

#### Server Usage

To listen on port 7028 for incoming OSC packets use the following command:

	$ oscchief listen 7028


## Credits

oscchief is based on the oscdump and oscsend tools from the liblo library written by
Kentaro Fukuchi.


## Licensing

oscchief is licensed under the terms of [The MIT License (MIT)](http://opensource.org/licenses/MIT).

	/* oscchief - Sends and receives OSC messages. 
	 *
	 * Copyright (c) 2013 Sebastian Ruml
	 * 
	 * Permission is hereby granted, free of charge, to any person obtaining a copy of
	 * this software and associated documentation files (the "Software"), to deal in
	 * the Software without restriction, including without limitation the rights to
	 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
	 * of the Software, and to permit persons to whom the Software is furnished to do
	 * so, subject to the following conditions:
	 *
	 * The above copyright notice and this permission notice shall be included in all
	 * copies or substantial portions of the Software.
	 *
	 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
	 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
	 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
	 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
	 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
	 */