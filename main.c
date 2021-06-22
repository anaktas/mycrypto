#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"
#include "aes.h"

void main_entry(Arguments *arguments);

int main(int argc, char *argv[]) {
    Arguments arguments;

    int parsing_error = parse_arguments(&arguments, argc, argv);

    if (parsing_error != 0) {
        printf("Error while parsing arguments.\n");
        return 1;
    }

    main_entry(&arguments);

    return 0;
}

void main_entry(Arguments *arguments) {
    uint8_t *input;
    uint8_t *output;
    uint8_t *key;

    long int input_file_size = 0;
    long int key_file_size = 0;

    input_file_size = get_file_size(arguments->input_file_path);
    if (input_file_size == 0){
        input_file_size = 16;
    }

    key_file_size = get_file_size(arguments->key_file_path);
    if (key_file_size == 0){
        key_file_size = 16;
    }

    int chunk_count = input_file_size / 16;
    int chunk_remnant = input_file_size % 16;
    printf("Input file size:              %ld\n", input_file_size);
    printf("Input variable chunk count:   %d\n", chunk_count);
    printf("Input variable chunk remnant: %d\n", chunk_remnant);

    input = (uint8_t*) malloc(sizeof(uint8_t) * input_file_size);
    output = (uint8_t*) malloc(sizeof(uint8_t) * input_file_size);
    key = (uint8_t*) malloc(sizeof(uint8_t) * key_file_size);
        
    int result = read_file(arguments->input_file_path, input, input_file_size);
    printf("The output is:\n%s\n", input);
    result = read_file(arguments->key_file_path, key, key_file_size);
    printf("The key is: %s\n", key);

    // size_t output_len = strlen(output);
    // size_t chunk_len = sizeof(chunk);

    strcpy(output, "");

    // TODO: We have to take into account the remnant in order
    // to make a proper encrypted file. Otherwise the decrypted
    // file will be a mess. -TD
    for (int i = 0; i < chunk_count; i++) {
        uint8_t *chunk = (uint8_t*) malloc(sizeof(uint8_t) * 16);

        memcpy(chunk, input + (i * sizeof(uint8_t) * 16), (sizeof(uint8_t) * 16));
        printf("Chunk %d:           %s\n", i, chunk);

        Aes aes;
        init(&aes, key, arguments->key_length);

        if (arguments->encryption_flag == 1) {
            uint8_t *cipher_out = (uint8_t*) malloc(sizeof(uint8_t) * 16);
            encrypt(&aes, chunk, cipher_out);
            printf("Encrypted chunk %d: %s\n", i, cipher_out);
            strcat(output, cipher_out);
            free(cipher_out);
        } else {
            uint8_t *decipher_out = (uint8_t*) malloc(sizeof(uint8_t) * 16);
            decrypt(&aes, chunk, decipher_out);
            printf("Decrypted chunk %d: %s\n", i, decipher_out);
            strcat(output, decipher_out);
            free(decipher_out);
        }
        
        printf("\n");

        free(chunk);
    }

    if (arguments->encryption_flag == 1) {
        write_file(output, "encrypted");
    } else {
        write_file(output, "decrypted");
    }

    free(input);
    free(output);
}