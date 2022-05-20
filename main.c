#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"
#include "aes.h"
#include "log.h"
#include "crypto_operations.h"
#include "config.h"

void
main_entry(Arguments *arguments);

int
main(int argc, char *argv[])
{
	inf("AES cryptographic tool v%s", VERSION);

    Arguments arguments;

    int parsing_error = parse_arguments(
        &arguments,
        argc,
        argv
    );

    if (0 != parsing_error)
    {
        err("Error while parsing arguments.");
        return 1;
    }

	printf("Enter password (password will be visible): ");
	scanf("%s", arguments.password);

	int len = strlen(arguments.password);
	dbg("Password length: %d", len);

    hash_password(arguments.key, arguments.password, len);

    print_hex_key(arguments.key);

    main_entry(&arguments);

    return 0;
}

void
main_entry(Arguments *arguments)
{
	int status = crypto_update_file(arguments);

    exit(status);
}