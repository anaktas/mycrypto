#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
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
	dbg("create_temp_file_with_padding(%s)", original_file_path);

	int exit_status = 0;

	uint8_t padding[16] = {0};

	char copy_cmd[512];

	sprintf(
		copy_cmd,
		"cp %s file.temp",
		original_file_path
	);

	dbg("Executing: %s", copy_cmd);
	int execution_status = system(copy_cmd);
	dbg("Execution status: %d", execution_status);

	FILE *temp_file = NULL;

	temp_file = fopen("file.temp", "ab");

	if (!temp_file) 
	{
		err("Unable to open temp file for appedning padding.");
		exit_status = 1;
		goto exit;
	}

	fwrite(padding,
		   sizeof(padding),
		   1,
		   temp_file);

	if (ferror(temp_file))
	{
		err("Unable to write in temp file.");
		exit_status = 1;
	}
	
exit:
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
	dbg("crypto_update_file()");

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

	if (is_encrypt)
	{
		if (create_temp_file_with_padding(args->input_file_path))
		{
			err("Unable to create temp file with padding.");
			exit_status = 1;
			goto exit;
		}

		input_file = fopen("file.temp", "rb");
		output_file = fopen(args->output_file_path, "wb");
	}
	else
	{
		input_file = fopen(args->input_file_path, "rb");
		output_file = fopen("file.temp", "wb");
	}

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

	if (!is_encrypt)
	{
		if (output_file)
		{
			fclose(output_file);
			output_file = NULL;
		}

		char head_cmd[512];

		sprintf(
			head_cmd,
			"head -c -16 file.temp > %s",
			args->output_file_path
		);

		/* @Refactor: We need a better way to do this */
		dbg("Executing: %s", head_cmd);
		int execution_status = system(head_cmd);
		dbg("Execution status: %d", execution_status);
	}

exit:
	if (remove("file.temp")) warn("Unable to remove file.temp");
	if (input_file) fclose(input_file);
	if (output_file) fclose(output_file);
	return exit_status;
}