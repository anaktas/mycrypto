#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"
#include "aes.h"

int main(int argc, char *argv[]) {
    char *output;

    if (argc == 2) {
        char *file_path = argv[1];
        long int file_size = 0;

        file_size = get_file_size(file_path);
        if (file_size == 0){
            file_size = 16;
        }

        int chunk_count = file_size / 16;
        int chunk_remnant = file_size % 16;
        printf("Output variable chunk count: %d\n", chunk_count);
        printf("Output variable chunk remnant: %d\n", chunk_remnant);

        output = (char*) malloc(sizeof(char) * file_size);
        
        int result = read_file(file_path, output, file_size);
        printf("The output is:\n%s\n", output);

        char *chunk = (char*) malloc(sizeof(char) * 16);

        // size_t output_len = strlen(output);
        // size_t chunk_len = sizeof(chunk);

        for (int i = 0; i < chunk_count; i++) {
            memcpy(chunk, output + (i * sizeof(char) * 16), (sizeof(char) * 16));
            printf("Chunk %d:           %s\n", i, chunk);

            Aes aes;
            init(&aes, "123456", 32);
            char *cipher_out = (char*) malloc(sizeof(char) * 16);
            encrypt(&aes, chunk, cipher_out);
            printf("Encrypted chunk %d: %s\n", i, cipher_out);
            char *decipher_out = (char*) malloc(sizeof(char) * 16);
            decrypt(&aes, cipher_out, decipher_out);
            printf("Decrypted chunk %d: %s\n", i, decipher_out);
            printf("\n");

            free(cipher_out);
            free(decipher_out);
        }

        free(chunk);

        write_file(output, "output");
    }

    free(output);

    return 0;
}