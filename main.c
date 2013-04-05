//
// This part is part of the OscChief project.
//
// Copyright 2013, Sebastian Ruml <sebastian@sebastianruml.com>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <errno.h>
#include <lo/lo.h>

#define VERSION "0.0.1"


/**
 * Prints the usage text.
*/
void usage()
{
	printf("oscchief Version %s\n", VERSION);
	printf("Copyright (C) 2013 Sebastian Ruml <sebastian.ruml@gmail.com>\n");
	printf("usage: oscchief HOST PORT OSCADDRESS TYPES ARGUMENTS\n\n");
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
	printf("Examples:\n");
	printf("    oscchief 192.168.0.10 7028 /osc/address ssiiii some integers 10 12 8 786\n");
	printf("    oscchief 192.168.0.10 7028 /osc/address TTiFi 643 98\n");
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

	// Arg 1: Hostname
	// Arg 2: OSC Port
	// Arg 3: OSC Address
	// Arg 4: Type Tags
	// Arg 5: Arguments

	// Build a new message
	message = lo_message_new();

	// Get the length of the type tags
	int numberOfArgs;
	if (argv[4] == NULL)
	{
		// An empty type string is allowed
		numberOfArgs = 0;
	}
	else
	{
		types = argv[4];
		numberOfArgs = strlen(types);
	}

	// Handle every argument
	int index = 5;
	char const *arg;
	for (int i = 0; i < numberOfArgs; ++i)
	{
		arg = argv[index];
		if (arg == NULL)
		{
			fprintf(stderr, "Argument %d not found.\n", i + 1);
			goto EXIT;
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
				if (lo_message_add_nil(message) < 0)
				{
					fprintf(stderr, "Could not add argument %d to the OSC message.\n", i+1);
					goto EXIT;
				}
			}
			break;

			case LO_NIL:
			{
				if (lo_message_add_true(message) < 0)
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
	if (argc < 4)
	{
		usage();
		exit(1);
	}

	// TODO: Check if got some optional arguments

	if (argv[1] == NULL)
	{
		fprintf(stderr, "No hostname is given.\n");
		exit(1);
	}

	if (argv[2] == NULL)
	{
		fprintf(stderr, "No port is specified.\n");
		exit(1);
	}

	// Create a new address
	lo_address target = lo_address_new(argv[1], argv[2]);
	if (target == NULL)
	{
		fprintf(stderr, "Failed to create %s:%s\n", argv[1], argv[2]);
		exit(1);
	}

	if (argv[3] == NULL)
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


	int ret = lo_send_message(target, argv[3], message);
	if (ret <= -1)
	{
		fprintf(stderr, "Error sending message.\n");
		exit(1);
	}
}
