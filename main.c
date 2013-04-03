//
// This part is part of the OscChief project.
//
// Copyright 2013, Sebastian Ruml <sebastian@sebastianruml.com>
//

#include <stdio.h>
#include <stdlib.h>
#include <lo/lo.h>

#define VERSION "0.0.1"


/**
 * Prints the usage.
*/
void usage()
{
	printf("oscchief Version %s\n", VERSION);
	printf("Copyright (C) 2013 Sebastian Ruml <sebastian.ruml@gmail.com>\n");
	printf("usage: oscchief HOST PORT OSCADDRESS TYPES ARGUMENTS\n\n");
	printf("positional arguments:\n");
	printf("    HOST: IP address of the host where you want to send your OSC message\n");
	printf("    PORT: TODO\n");
	printf("    OSCADDRESS: TODO\n");
	printf("    TYPES: OSC type tags. Supported types:\n");
	printf("      i: Integer 32 Bit\n");
	printf("      i: Integer 32 Bit\n");
	printf("      i: Integer 32 Bit\n");
	printf("      i: Integer 32 Bit\n");
	printf("      i: Integer 32 Bit\n");
	printf("      i: Integer 32 Bit\n");
	printf("      i: Integer 32 Bit\n");
	printf("      i: Integer 32 Bit\n\n");
	printf("Example usage:\n");
	printf("    oscchief 192.168.0.10 7028 /osc/address ssiiii some integers 10 12 8 786\n");
}

/**
 * Creates a new OSC message from the arguments and type tags.
 *
 * \returns The created OSC message.
 */
lo_message create_message(char **argv)
{
	lo_message message;

	// Arg 1: Hostname
	// Arg 2: OSC Port
	// Arg 3: OSC Address
	// Arg 4: Type Tags
	// Arg 5: Arguments

	// Build a new message
	message = lo_message_new();

	// Get the length of the type tags

	// Handle every argument (handle every OSC type)

	return message;
}

int main(int argc, char **argv)
{
	if (argc < 4)
	{
		usage();
		exit(1);
	}

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
