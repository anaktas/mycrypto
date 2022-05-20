#include <stdint.h>
#include <stdbool.h>
#include "utils.h"
#include "aes.h"
#include "log.h"

#define BUFFER_SIZE 16

/**
 * @brief Creates a temporary file which has the same
 * contents as the original file + 16 bytes of padding
 * at the end of the file.
 * 
 * @param original_file_path the given file path of the
 * 							 original file
 * 
 * @return int 0 SUCCESS, 1 FAILURE
 */
static int
create_temp_file_with_padding(char *original_file_path)
{
	int exit_status = 0;

	uint8_t padding[16] = {0};
	uint8_t buffer[BUFFER_SIZE];

	FILE *original_file = NULL;
	FILE *intermediate_temp_file = NULL;
	FILE *temp_file = NULL;

	original_file = fopen(original_file_path, "rb");

	if (!original_file)
	{
		err("Unable to open the input file.");
		exit_status = 1;
		goto exit;
	}

	intermediate_temp_file = fopen("file.temp", "wb");

	if (!intermediate_temp_file)
	{
		err("Unable to open the intermediate temp file.");
		exit_status = 1;
		goto exit;
	}

	int number_of_bytes = 0;

	while (1)
	{
		number_of_bytes = fread(buffer,
								sizeof(uint8_t),
								sizeof(buffer),
								original_file);

		if (ferror(original_file))
		{
			err("Failed to read from input file.");
			exit_status = 1;
			goto exit;
		}

		fwrite(buffer,
			   sizeof(uint8_t),
			   number_of_bytes,
			   intermediate_temp_file);

		if (ferror(intermediate_temp_file))
		{
			err("Unable to write in output file.");
			exit_status = 1;
			goto exit;
		}

		if (number_of_bytes < BUFFER_SIZE) break;
	}

	if (intermediate_temp_file)
	{
		fclose(intermediate_temp_file);
		intermediate_temp_file = NULL;
	}

	temp_file = fopen("file.temp", "a");

	if (!temp_file) 
	{
		err("Unable to open temp file for appedning padding.");
		exit_status = 1;
		goto exit;
	}

	fwrite(padding,
		   sizeof(padding),
		   BUFFER_SIZE,
		   temp_file);

	if (ferror(temp_file))
	{
		err("Unable to write in temp file.");
		exit_status = 1;
	}
	
exit:
	if (original_file) fclose(original_file);
	if (intermediate_temp_file) fclose(intermediate_temp_file);
	if (temp_file) fclose(temp_file);
	return exit_status;
}

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