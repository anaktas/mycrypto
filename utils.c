#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include "utils.h"
#include "log.h"
#include "config.h"

/**
 * @brief Constant magic key seed array with random selected bytes
 */
static const uint8_t key_seed[32] =
{
	0x88, 0x46, 0xEE, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x5B, 0x6A, 0xCB, 0xD7, 0xAB, 0x76,
	0x04, 0xC7, 0x23, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0
};

/**
 * @brief Debugging function. Prints out the key seed and the
 * hashed password.
 * 
 * @param key the given byte key to print
 */
void
print_hex_key(uint8_t *key)
{
#ifdef ENABLE_DEBUG
    printf("\n");

    printf("Key seed (hex): ");
    for (int i = 0; i < 32; i++)
    {
        printf("%02hhx ", key_seed[i]);
    }

    printf("\n");

    printf("Key (hex):      ");
    for (int i = 0; i < 32; i++)
    {
        printf("%02hhx ", key[i]);
    }

    printf("\n");
#endif
}

/**
 * @brief Converts a string password into a 
 * usable encryption key.
 * 
 * @param key      the output byte key
 * @param password the given passowrd
 * @param len      the length of the password
 */
void
hash_password(uint8_t *key,
              char *password,
              int len)
{
    int offset = 0;

    for (int i = 0; i < 32; i++)
    {
        int pass_index = i - offset;

        if (pass_index >= len)
        {
            pass_index = 0;
            offset = i;
        }

        key[i] = key_seed[i] ^ ((uint8_t) password[pass_index]);
    }
}

/**
 * Reads a file from a file path and assigns it's content to
 * an output pointer. The buffer size has to be given as an
 * input as well
 * 
 * @param file_path the file path
 * @param output the output char pointer
 * @param output_size the size of the given output array
 */
int
read_file(char *file_path,
          char *output,
          long int output_size)
{
    FILE *file;

    file = fopen(file_path, "r");

    if (file == NULL)
    {
        err("Error while opening the file in read_file()");
        return 1;
    }

    //fseek(file, 0, SEEK_END);
    //fseek(file, 0, SEEK_SET);
    fread(output, 1, output_size, file);

    fclose(file);

    return 0;
}

/**
 * Writes a file to a given file path.
 * 
 * @param output the given output to store to a file
 * @param file_path the file path to store the file
 */
int
write_file(char *output, char *file_path)
{
    FILE *file;
    int result = 0;

    file = fopen(file_path, "w+");

    if (file == NULL)
    {
        err("Error while opening the file in write_file()");
        return 1;
    }
    
    result = fputs(output, file);

    return result;
}

/**
 * Gets the byte count of a file.
 * 
 * @param file_path the file path
 */
long int
get_file_size(char *file_path)
{
    FILE *file;
    long int file_size = 0;

    file = fopen(file_path, "r");

    if (file == NULL)
    {
        err("Error while opening the file in get_file_size()");
    }

    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    fclose(file);

    return file_size;
}

/**
 * This function is parsing the console arguments into
 * a local arguments structure.
 * 
 * @param args an Arguments struct pointer
 * @param argc the arguments counter
 * @param argv the arguments vector
 */
int
parse_arguments(Arguments *args,
                int argc,
                char *argv[])
{
    if (argc < 7)
    {
        err("Not enough arguments.");
        err("Usage: aes [-e|-d] -i <INPUT_FILE_PATH> -o <OUTPUT_FILE_PATH> -k <KEY_FILE_PATH> -l [128|192|256]");
        return 1;
    }

    args->encryption_flag = 0;
    args->input_file_path = "";
	args->output_file_path = "";
    args->key_file_path = "";
    args->key_length = 16;

    for (int i = 1; i < argc; i++)
    {
        if ((i + 1) == argc) break;

        if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "-e"))
        {
            if (!strcmp(argv[i], "-d"))
            {
                args->encryption_flag = 0;
            }
            else
            {
                args->encryption_flag = 1;
            }
        }

		if (!strcmp(argv[i], "-i"))
		{
			args->input_file_path = argv[i + 1];
		}
		
		if (!strcmp(argv[i], "-o"))
		{
			args->output_file_path = argv[i + 1];
		}

        if (!strcmp(argv[i], "-k"))
        {
            args->key_file_path = argv[i + 1];
        }

        if (!strcmp(argv[i], "-l"))
        {
            if (!strcmp(argv[i + 1], "128")) args->key_length = 16;
            if (!strcmp(argv[i + 1], "192")) args->key_length = 24;
            if (!strcmp(argv[i + 1], "256")) args->key_length = 32;
        }
    }

    return 0;    
}

/**
 * @brief Performs an assertion of a given
 * expression. Should be invoked only in
 * debug mode
 * 
 * @param assert_result the result of an assertion expression 
 * @param explanation a message to display before the assertion
 */
void
check(int assert_result, char *explanation)
{
#ifdef ENABLE_DEBUG
    dbg(explanation);
    assert(assert_result);
#endif
}

/**
 * @brief Converts a given byte array into
 * a hex represenation string of each byte.
 * 
 * @param out     the output string
 * @param in      the input byte array
 * @param out_len the output string length
 * @param in_len  the input array size
 */
void
convert_to_hex_string(char *out, 
                      uint8_t *in, 
                      size_t out_len, 
                      size_t in_len)
{
    static const char hex_table[] = "0123456789abcdef";

    if (out_len < ((in_len * 2) +1))
    {
        err("Error while converting to hex string");
        return;
    }

    while (in_len--)
    {
        /* shift down the top nibble and pick a char from the hex_table */
        *out++ = hex_table[*in >> 4];
        /* extract the bottom nibble and pick a char from the hex_table */
        *out++ = hex_table[*in++ & 0xF];
    }

    *out = 0;
}

/**
 * @brief Converts a given hex representation string
 * into a byte array.
 * 
 * @param out     the output byte array
 * @param in      the input hex string
 * @param in_size the string length
 */
void
convert_to_uint8_array(uint8_t *out, 
                       char *in, 
                       size_t in_size) 
{
    char buffer[3];
    size_t i;
    int value;

    for (i = 0; i < in_size && *in; i++) 
    {
        buffer[0] = *in++;
        buffer[1] = '\0';

        if (*in) 
        {
            buffer[1] = *in++;
            buffer[2] = '\0';
        }
        
        if (sscanf(buffer, "%x", &value) != 1) 
        {
            break;
        }

        out[i] = value;
    }
}