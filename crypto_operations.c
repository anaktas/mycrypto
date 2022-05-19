#include <stdint.h>
#include <stdbool.h>
#include "utils.h"
#include "aes.h"
#include "log.h"

#define BUFFER_SIZE 16

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
	int exit_status = 0;

	if (is_encrypt)
	{
		inf("Encrypting file: %s", args->input_file_path);
	}
	else
	{
		inf("Decrypting file: %s", args->input_file_path);
	}

	check(args->input_file_path != NULL, "Check input file path");
	check(args->output_file_path != NULL, "Check output file path");

	int number_of_bytes;
	uint8_t input_buffer[BUFFER_SIZE], output_buffer[BUFFER_SIZE];

	FILE *input_file = NULL;
	FILE *output_file = NULL;

	Aes aes;

	input_file = fopen(args->input_file_path, "rb");
	output_file = fopen(args->output_file_path, "wb");

	if (!input_file)
	{
		err("Unable to open: %s in reading mode.", args->input_file_path);
		exit_status = 1;
		goto exit;
	}

	if (!output_file)
	{
		err("Unabe to open: %s in writing mode.", args->output_file_path);
		exit_status = 1;
		goto exit;
	}

	Aes_Error aes_error = init(&aes, args->key, args->key_length);

	if (aes_error != NO_ERROR)
	{
		err("Unable to initialize the AES key. Code: %d", aes_error);
		exit_status = 1;
		goto exit;
	}
	
	int count = 0;

	while (1)
	{
		number_of_bytes = fread(input_buffer,
								sizeof(uint8_t),
								sizeof(input_buffer),
								input_file);

		dbg("Loop index %d, n. of bytes %d", count, number_of_bytes);

		count++;

		if (ferror(input_file))
		{
			err("Failed to read from input file.");
			exit_status = 1;
			goto exit;
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
			   number_of_bytes,
			   output_file);

		if (ferror(output_file))
		{
			err("Unable to write in output file.");
			exit_status = 1;
			goto exit;
		}

		if (number_of_bytes < BUFFER_SIZE) break;
	}

	inf("The %scryption of the file has been completed.", (is_encrypt ? "en" : "de"));

exit:
	if (input_file) fclose(input_file);
	if (output_file) fclose(output_file);
	return exit_status;
}