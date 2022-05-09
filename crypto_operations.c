#include <stdint.h>
#include <stdbool.h>
#include "utils.h"
#include "aes.h"
#include "log.h"

/**
 * @brief
 * 
 * File encryption/decryption routine wrapper.
 * 
 * @param args       given command line arguments
 * @param is_encrypt encryption flag
 * 
 * @return int 
 */
int
crypto_update_file(Arguments *args, bool is_encrypt)
{
	if (is_encrypt)
	{
		inf("Encrypting file: %s", args->input_file_path);
	}
	else
	{
		inf("Decrypting file: %s", args->input_file_path);
	}

	check(args->input_file_path != NULL, "Check input file path");
	check(args->output_file_path != NULL, "Chekc output file path");

	int number_of_bytes;
	uint8_t input_buffer[16], output_buffer[16];

	FILE *input_file = NULL;
	FILE *output_file = NULL;

	Aes aes;

	input_file = fopen(args->input_file_path, "rb");
	output_file = fopen(args->output_file_path, "wb");

	if (!input_file)
	{
		err("Unable to open: %s in reading mode.", args->input_file_path);
		goto error;
	}

	if (!output_file)
	{
		err("Unabe to open: %s in writing mode.", args->output_file_path);
		goto error;
	}

	Aes_Error aes_error = init(&aes, args->key, args->key_length);

	if (aes_error != NO_ERROR)
	{
		err("Unable to initialize the AES key. Code: %d", aes_error);
		goto error;
	}

	printf("\n");

	while (1)
	{
		printf(".");

		number_of_bytes = fread(input_buffer,
								sizeof(uint8_t),
								16,
								input_file);

		if (ferror(input_file))
		{
			err("Failed to read from input file.");
			goto error;
		}

		if (is_encrypt)
		{
			encrypt(&aes, input_buffer, output_buffer);
		}
		else
		{
			decrypt(&aes, input_buffer, output_buffer);
		}

		fwrite(output_buffer,
			   sizeof(uint8_t),
			   sizeof(output_buffer),
			   output_file);

		if (ferror(output_file))
		{
			err("Unable to write in output file.");
			goto error;
		}

		if (number_of_bytes < 16) break;
	}

	printf("\n");

	return 0;

error:
	printf("\n");
	if (input_file) fclose(input_file);
	if (output_file) fclose(output_file);
	return 1;
}