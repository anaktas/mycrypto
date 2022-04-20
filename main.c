#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"
#include "aes.h"

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
        printf("Error while parsing arguments.\n");
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
        printf("It seems that the input file is empty. Aborting operation.\n");
        return;
    }

    key_file_size = get_file_size(arguments->key_file_path);

    if (key_file_size == 0)
    {
        printf("It seems that the key file is empty. Aborting operation.\n");
        return;
    }

    int chunk_count = input_file_size / 16;
    int chunk_remnant = input_file_size % 16;

    printf("Input file size:              %ld\n", input_file_size);
    printf("Input variable chunk count:   %d\n", chunk_count);
    printf("Input variable chunk remnant: %d\n", chunk_remnant);

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
        printf("Starting encryption...\n");
    }
    else
    {
        printf("Starting decryption...\n");
    }

    for (int i = 0; i < chunk_count; i++)
    {
        printf(".");
        char chunk[16 + 1];

        memcpy(chunk, input + (i * sizeof(char) * 16), (sizeof(char) * 16));
        chunk[16] = '\0';
        // printf("Chunk %d:           %s\n", i, chunk);
        // printf("Strlen chunk:       %ld\n", strlen(chunk));

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
            // printf("Encrypted chunk %d: %s\n", i, cipher_out);
            // printf("Strlen encrypted:   %ld\n", strlen(cipher_out));

            // for (int j = 0; j < 16; j++)
            // {
            //     printf("Cipher out %d: %c\n", j, cipher_out[j]);
            // }

            // decrypt(&aes, cipher_out, decipher_out);
            // printf("Decrypted chunk %d: %s\n", i, decipher_out);
            // printf("Strlen decrypted:   %ld\n", strlen(decipher_out));

            // for (int j = 0; j < 16; j++)
            // {
            //     printf("Decipher out %d: %c\n", j, decipher_out[j]);
            // }

            strcat(output, cipher_out);
        }
        else
        {
            char decipher_out[16 + 1];
            decipher_out[16] = '\0';
            decrypt(&aes, chunk, decipher_out);
            // printf("Decrypted chunk %d: %s\n", i, decipher_out);
            strcat(output, decipher_out);
        }
    }

    if (chunk_remnant != 0)
    {
        printf(".");
        char chunk[16 + 1];

        memcpy(chunk, input + (chunk_count * sizeof(char) * 16), (sizeof(char) * 16));
        chunk[16] = '\0';
        // printf("Chunk %d:           %s\n", chunk_count, chunk);

        if (arguments->encryption_flag == 1)
        {
            char cipher_out[16 + 1];
            cipher_out[16] = '\0';
            char decipher_out[16 + 1];
            decipher_out[16] = '\0';
            encrypt(&aes, chunk, cipher_out);
            // printf("Encrypted chunk %d: %s\n", chunk_count, cipher_out);
            // printf("Strlen encrypted:   %ld\n", strlen(cipher_out));

            // for (int j = 0; j < 16; j++) {
            //     printf("Cipher out %d: %c\n", j, cipher_out[j]);
            // }

            // decrypt(&aes, cipher_out, decipher_out);
            // printf("Decrypted chunk %d: %s\n", chunk_count, decipher_out);
            // printf("Strlen decrypted:   %ld\n", strlen(decipher_out));

            // for (int j = 0; j < 16; j++) {
            //     printf("Decipher out %d: %c\n", j, decipher_out[j]);
            // }

            strcat(output, cipher_out);
        }
        else
        {
            char decipher_out[16 + 1];
            decipher_out[16] = '\0';
            decrypt(&aes, chunk, decipher_out);
            //printf("Decrypted chunk %d: %s\n", chunk_count, decipher_out);
            strcat(output, decipher_out);
        }
    }

    printf("\n");

    if (arguments->encryption_flag == 1)
    {
        printf("Saving encrypted file...\n");
        write_file(output, "encrypted");
    }
    else
    {
        printf("Saving decrypted file...\n");
        write_file(output, "decrypted");
    }

    printf("Done.\n");

    free(input);
    free(output);
}