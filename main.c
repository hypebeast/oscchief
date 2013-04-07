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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <errno.h>
#include <stdbool.h>
#include <lo/lo.h>

#define VERSION "0.1.0"


/**
 * Prints the usage text.
*/
void usage()
{
	printf("oscchief Version %s\n", VERSION);
	printf("Copyright (C) 2013 Sebastian Ruml <sebastian.ruml@gmail.com>\n\n");
	printf("usage: oscchief send HOST PORT OSCADDRESS TYPES ARGUMENTS\n");
	printf("       oscchief listen PORT\n\n");
	printf("positional arguments:\n");
	printf("    HOST: IP address of the host where you want to send your OSC message\n");
	printf("    PORT: Port number\n");
	printf("    OSCADDRESS: OSC address where you want to send your message\n");
	printf("    TYPES: OSC type tags. Supported types:\n");
	printf("        %c - 32 Bit integer\n", LO_INT32);
	printf("        %c - 64 Bit integer\n", LO_INT64);
	printf("        %c - 32 Bit float\n", LO_FLOAT);
	printf("        %c - 64 Bit double\n", LO_DOUBLE);
	printf("        %c - Char\n", LO_CHAR);
	printf("        %c - String\n", LO_STRING);
	printf("        %c - True (no argument required)\n", LO_TRUE);
	printf("        %c - False (no argument required)\n", LO_FALSE);
	printf("        %c - Nil (no argument required)\n\n", LO_NIL);
	printf("optional arguments:\n");
	printf("     -h - Shows this help message\n\n");
	printf("Examples:\n");
	printf("    oscchief send 192.168.0.10 7028 /osc/address %c%c%c%c%c some integers 10 12 786\n",
			LO_STRING, LO_STRING, LO_INT32, LO_INT32, LO_INT32);
	printf("    oscchief send 192.168.0.10 7028 /osc/address %c%c%c%c%c 643 98\n",
			LO_TRUE, LO_TRUE, LO_INT32, LO_FALSE, LO_INT32);
	printf("    oscchief send 192.168.0.10 7028 /osc/address\n");
	printf("    oscchief send 192.168.0.10 7028 /osc/address %c%c\n", LO_TRUE, LO_FALSE);
	printf("    oscchief listen 7028\n");
}

void errorHandler(int num, const char *msg, const char *where)
{
	printf("Server error %d in address %s: %s\n", num, msg, where);
}

/**
 * Message handler for incoming OSC messages.
 */
int messageHandler(const char *address, const char *types, lo_arg **argv, int argc, lo_message msg, void *user_data)
{
	printf("%s %s", address, types);

	for (int i = 0; i < argc; i++)
	{
		printf(" ");
		lo_arg_pp((lo_type)types[i], argv[i]);
	}
	printf("\n");

	return 0;
}

/**
 * Creates a new OSC message from the arguments and type tags.
 *
 * \returns The created OSC message.
 */
lo_message create_message(char **argv)
{
	char const *types;
	lo_message message;

	// Arg 2: Hostname
	// Arg 3: OSC Port
	// Arg 4: OSC Address
	// Arg 5: Type Tags
	// Arg 6: Arguments

	// Build a new message
	message = lo_message_new();

	// Get the length of the type tags
	int numberOfArgs;
	if (argv[5] == NULL)
	{
		// An empty type string is allowed
		numberOfArgs = 0;
	}
	else
	{
		types = argv[5];
		numberOfArgs = strlen(types);
	}

	// Handle every argument
	int index = 6;
	char const *arg;
	for (int i = 0; i < numberOfArgs; i++)
	{
		if (types[i] != LO_FALSE && types[i] != LO_TRUE
			&& types[i] != LO_NIL)
		{
			arg = argv[index];
			if (arg == NULL)
			{
				fprintf(stderr, "Argument %d not found.\n", i + 1);
				goto EXIT;
			}	
		}

		switch(types[i])
		{
			case LO_INT32:
			{
				int64_t val;
				char *endptr;

				errno = 0;
				val = strtol(arg, &endptr, 10);
				if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
					|| (errno != 0 && val == 0))
				{
					fprintf(stderr, "Could not parse argument %d '%s' as INT32.\n", i+1, arg);
					goto EXIT;
				}

				if (val > INT_MAX || val < INT_MIN)
				{
					fprintf(stderr, "Could not parse argument %d '%s' as INT32.\n", i+1, arg);
					goto EXIT;
				}

				if (endptr == arg)
				{
					fprintf(stderr, "Could not parse argument %d '%s' as INT32.\n", i+1, arg);
					goto EXIT;
				}

				if (lo_message_add_int32(message, (int32_t)val) < 0)
				{
					fprintf(stderr, "Could not add argument %d '%s' to the OSC message.\n", i+1, arg);
					goto EXIT;	
				}
				index++;
			}
			break;

			case LO_INT64:
			{
				int64_t val;
				char *endptr;

				errno = 0;
				val = strtol(arg, &endptr, 10);
				if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
					|| (errno != 0 && val == 0))
				{
					fprintf(stderr, "Could not parse argument %d '%s' as INT64.\n", i+1, arg);
					goto EXIT;
				}

				if (val > LONG_MAX || val < LONG_MIN)
				{
					fprintf(stderr, "Could not parse argument %d '%s' as INT64.\n", i+1, arg);
					goto EXIT;
				}

				if (endptr == arg)
				{
					fprintf(stderr, "Could not parse argument %d '%s' as INT64.\n", i+1, arg);
					goto EXIT;
				}

				if (lo_message_add_int64(message, val) < 0)
				{
					fprintf(stderr, "Could not add argument %d '%s' to the OSC message.\n", i+1, arg);
					goto EXIT;	
				}
				index++;
			}
			break;

			case LO_FLOAT:
			{
				float val;
				char *endptr;

				errno = 0;
				val = strtof(arg, &endptr);
				if ((errno == ERANGE && val == HUGE_VALF)
					|| (errno == ERANGE && val == 0))
				{
					fprintf(stderr, "Could not parse argument %d '%s' as FLOAT.\n", i+1, arg);
					goto EXIT;
				}

				if (endptr == arg)
				{
					fprintf(stderr, "Could not parse argument %d '%s' as FLOAT.\n", i+1, arg);
					goto EXIT;
				}

				if (lo_message_add_float(message, val) < 0)
				{
					fprintf(stderr, "Could not add argument %d '%s' to the OSC message.\n", i+1, arg);
					goto EXIT;	
				}
				index++;
			}
			break;

			case LO_DOUBLE:
			{
				double val;
				char *endptr;

				errno = 0;
				val = strtod(arg, &endptr);
				if ((errno == ERANGE && val == HUGE_VAL)
					|| (errno == ERANGE && val == 0))
				{
					fprintf(stderr, "Could not parse argument %d '%s' as DOUBLE.\n", i+1, arg);
					goto EXIT;
				}

				if (endptr == arg)
				{
					fprintf(stderr, "Could not parse argument %d '%s' as DOUBLE.\n", i+1, arg);
					goto EXIT;
				}

				if (lo_message_add_double(message, val) < 0)
				{
					fprintf(stderr, "Could not add argument %d '%s' to the OSC message.\n", i+1, arg);
					goto EXIT;	
				}
				index++;
			}
			break;

			case LO_CHAR:
			{
				if (strlen(arg) != 1)
				{
					fprintf(stderr, "Only single byte char is allowed.\n");
					goto EXIT;
				}

				if (lo_message_add_char(message, arg[0]) < 0)
				{
					fprintf(stderr, "Could not add argument %d '%s' to the OSC message.\n", i+1, arg);
					goto EXIT;	
				}
				index++;
			}
			break;

			case LO_STRING:
			{
				if (lo_message_add_string(message, arg) < 0)
				{
					fprintf(stderr, "Could not add argument %d '%s' to the OSC message.\n", i+1, arg);
					goto EXIT;
				}
			}
			break;

			case LO_TRUE:
			{
				if (lo_message_add_true(message) < 0)
				{
					fprintf(stderr, "Could not add argument %d to the OSC message.\n", i+1);
					goto EXIT;
				}
			}
			break;

			case LO_FALSE:
			{
				if (lo_message_add_false(message) < 0)
				{
					fprintf(stderr, "Could not add argument %d to the OSC message.\n", i+1);
					goto EXIT;
				}
			}
			break;

			case LO_NIL:
			{
				if (lo_message_add_nil(message) < 0)
				{
					fprintf(stderr, "Could not add argument %d  to the OSC message.\n", i+1);
					goto EXIT;
				}
			}
			break;

			default:
				fprintf(stderr, "Type '%c' is not supported.\n", types[i]);
				goto EXIT;
		}
	}

	return message;

EXIT:
	lo_message_free(message);
	return NULL;
}

int main(int argc, char **argv)
{
	bool runAsServer = false;

	if (argc < 2)
	{
		usage();
		exit(1);
	}

	// Handle optional arguments
	char *firstArg = argv[1];
	if (strcmp(firstArg, "-h") == 0)
	{
		usage();
		exit(1);
	}
	else if (strcmp(firstArg, "send") == 0)
	{
		runAsServer = false;
	}
	else if (strcmp(firstArg, "listen") == 0)
	{
		runAsServer = true;
	}
	else
	{
		printf("Invalid argument(s)!\n");
		usage();
		exit(1);	
	}

	// Run as server and listen for incoming OSC messages
	if (runAsServer)
	{
		if (argc < 3)
		{
			usage();
			exit(1);
		}

		// Create new server
		lo_server server = lo_server_new(argv[2], errorHandler);
		if (server == NULL)
		{
			printf("Could not start a server with port %s", argv[3]);
			exit(1);
		}

		// Add a message handler that matches all OSC addresses
		lo_server_add_method(server, NULL, NULL, messageHandler, NULL);

		printf("Start listening on port '%s' for incoming OSC messages...\n", argv[2]);

		// Loop forever and receive OSC messages
		for (;;)
		{
			lo_server_recv(server);
		}
	}
	else // Run as client
	{
		if (argc < 5)
		{
			usage();
			exit(1);
		}

		if (argv[2] == NULL)
		{
			fprintf(stderr, "No hostname is given.\n");
			exit(1);
		}

		if (argv[3] == NULL)
		{
			fprintf(stderr, "No port is specified.\n");
			exit(1);
		}

		// Create a new address
		lo_address target = lo_address_new(argv[2], argv[3]);
		if (target == NULL)
		{
			fprintf(stderr, "Failed to create %s:%s\n", argv[2], argv[3]);
			exit(1);
		}

		if (argv[4] == NULL)
		{
			fprintf(stderr, "No OSC address is given\n");
			exit(1);
		}

		lo_message message = create_message(argv);
		if (message == NULL)
		{
			fprintf(stderr, "Could not create OSC message.\n");
			exit(1);
		}


		int ret = lo_send_message(target, argv[4], message);
		if (ret <= -1)
		{
			fprintf(stderr, "Error sending message.\n");
			exit(1);
		}
	}
}
