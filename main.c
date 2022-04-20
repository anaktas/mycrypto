#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"
#include "aes.h"
#include "log.h"

void
main_entry(Arguments *arguments);

int
main(int argc, char *argv[])
{
    Arguments arguments;

    int parsing_error = parse_arguments(
        &arguments,
        argc,
        argv
    );

    if (parsing_error != 0)
    {
        err("Error while parsing arguments.");
        return 1;
    }

    main_entry(&arguments);

    return 0;
}

void
main_entry(Arguments *arguments)
{
    char *input;
    char *output;
    char *key;

    long int input_file_size = 0;
    long int key_file_size = 0;

    input_file_size = get_file_size(arguments->input_file_path);

    if (input_file_size == 0)
    {
        err("It seems that the input file is empty. Aborting operation.");
        return;
    }

    key_file_size = get_file_size(arguments->key_file_path);

    if (key_file_size == 0)
    {
        err("It seems that the key file is empty. Aborting operation.");
        return;
    }

    int chunk_count = input_file_size / 16;
    int chunk_remnant = input_file_size % 16;

    dbg("Input file size:              %ld", input_file_size);
    dbg("Input variable chunk count:   %d", chunk_count);
    dbg("Input variable chunk remnant: %d", chunk_remnant);

    input = (char*) malloc(sizeof(char) * input_file_size);
    output = (char*) malloc(sizeof(char) * input_file_size);

    // We need to revisit the remnant handling here
    if (arguments->encryption_flag == 1)
    {
        if (chunk_remnant != 0)
        {
            free(input);
            free(output);
            input = (char*) malloc(sizeof(char) * (input_file_size + 16));
            output = (char*) malloc(sizeof(char) * (input_file_size + 16));
        }
    }

    key = (char*) malloc(sizeof(char) * key_file_size);
        
    int result = read_file(arguments->input_file_path, input, input_file_size);
    result = read_file(arguments->key_file_path, key, key_file_size);

    // size_t output_len = strlen(output);
    // size_t chunk_len = sizeof(chunk);

    strcpy(output, "");

    Aes aes;
    init(&aes, key, arguments->key_length);

    if (arguments->encryption_flag == 1)
    {
        inf("Starting encryption...");
    }
    else
    {
        inf("Starting decryption...");
    }

    for (int i = 0; i < chunk_count; i++)
    {
        inf(".");
        char chunk[16 + 1];

        memcpy(chunk, input + (i * sizeof(char) * 16), (sizeof(char) * 16));
        chunk[16] = '\0';
        // dbg("Chunk %d:           %s", i, chunk);
        // dbg("Strlen chunk:       %ld", strlen(chunk));

        if (arguments->encryption_flag == 1)
        {
            char cipher_out[16 + 1];
            cipher_out[16] = '\0';
            char decipher_out[16 + 1];
            decipher_out[16] = '\0';
            // Something really strange happens during the encryption.
            // It seems that in some occasion, the first or one of the
            // chunks will be encrypted in a string with less that the
            // expected length.
            encrypt(&aes, chunk, cipher_out);
            // dbg("Encrypted chunk %d: %s", i, cipher_out);
            // dbg("Strlen encrypted:   %ld", strlen(cipher_out));

            // for (int j = 0; j < 16; j++)
            // {
            //     dbg("Cipher out %d: %c", j, cipher_out[j]);
            // }

            // decrypt(&aes, cipher_out, decipher_out);
            // dbg("Decrypted chunk %d: %s", i, decipher_out);
            // dbg("Strlen decrypted:   %ld", strlen(decipher_out));

            // for (int j = 0; j < 16; j++)
            // {
            //     dbg("Decipher out %d: %c", j, decipher_out[j]);
            // }

            strcat(output, cipher_out);
        }
        else
        {
            char decipher_out[16 + 1];
            decipher_out[16] = '\0';
            decrypt(&aes, chunk, decipher_out);
            // dbg("Decrypted chunk %d: %s", i, decipher_out);
            strcat(output, decipher_out);
        }
    }

    if (chunk_remnant != 0)
    {
        inf(".");
        char chunk[16 + 1];

        memcpy(chunk, input + (chunk_count * sizeof(char) * 16), (sizeof(char) * 16));
        chunk[16] = '\0';
        // dbg("Chunk %d:           %s", chunk_count, chunk);

        if (arguments->encryption_flag == 1)
        {
            char cipher_out[16 + 1];
            cipher_out[16] = '\0';
            char decipher_out[16 + 1];
            decipher_out[16] = '\0';
            encrypt(&aes, chunk, cipher_out);
            // dbg("Encrypted chunk %d: %s", chunk_count, cipher_out);
            // dbg("Strlen encrypted:   %ld", strlen(cipher_out));

            // for (int j = 0; j < 16; j++) {
            //     dbg("Cipher out %d: %c", j, cipher_out[j]);
            // }

            // decrypt(&aes, cipher_out, decipher_out);
            // dbg("Decrypted chunk %d: %s", chunk_count, decipher_out);
            // dbg("Strlen decrypted:   %ld", strlen(decipher_out));

            // for (int j = 0; j < 16; j++) {
            //     dbg("Decipher out %d: %c", j, decipher_out[j]);
            // }

            strcat(output, cipher_out);
        }
        else
        {
            char decipher_out[16 + 1];
            decipher_out[16] = '\0';
            decrypt(&aes, chunk, decipher_out);
            //dbg("Decrypted chunk %d: %s", chunk_count, decipher_out);
            strcat(output, decipher_out);
        }
    }

    inf("\n");

    if (arguments->encryption_flag == 1)
    {
        inf("Saving encrypted file...");
        write_file(output, "encrypted");
    }
    else
    {
        inf("Saving decrypted file...");
        write_file(output, "decrypted");
    }

    inf("Done.");

    free(input);
    free(output);
}