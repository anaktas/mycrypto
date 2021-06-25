#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

/**
 * Reads a file from a file path and assigns it's content to
 * an output pointer. The buffer size has to be given as an
 * input as well
 * 
 * @param file_path the file path
 * @param output the output char pointer
 * @param output_size the size of the given output array
 */
int read_file(char *file_path, char *output, long int output_size) {
    FILE *file;

    file = fopen(file_path, "r");

    if (file == NULL) {
        perror("Error while opening the file in read_file()\n");
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
int write_file(char *output, char *file_path) {
    FILE *file;
    int result = 0;

    file = fopen(file_path, "w+");

    if (file == NULL) {
        perror("Error while opening the file in write_file()\n");
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
long int get_file_size(char *file_path) {
    FILE *file;
    long int file_size = 0;

    file = fopen(file_path, "r");

    if (file == NULL) {
        perror("Error while opening the file in get_file_size()\n");
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
int parse_arguments(Arguments *args, int argc, char *argv[]) {
    if (argc < 7) {
        printf("Not enough arguments.\n");
        printf("Usage: aes [-e|-d] <INPUT_FILE_PATH> -k <KEY_FILE_PATH> -l [128|192|256]\n");
        return 1;
    }

    args->encryption_flag = 0;
    args->input_file_path = "";
    args->key_file_path = "";
    args->key_length = 16;

    for (int i = 1; i < argc; i++) {
        if ((i + 1) == argc) break;

        if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "-e")) {
            if (!strcmp(argv[i], "-d")) {
                args->encryption_flag = 0;
            } else {
                args->encryption_flag = 1;
            }
            
            args->input_file_path = argv[i + 1];
        }

        if (!strcmp(argv[i], "-k")) {
            args->key_file_path = argv[i + 1];
        }

        if (!strcmp(argv[i], "-l")) {
            if (!strcmp(argv[i + 1], "128")) args->key_length = 16;
            if (!strcmp(argv[i + 1], "192")) args->key_length = 24;
            if (!strcmp(argv[i + 1], "256")) args->key_length = 32;
        }
    }

    return 0;    
}
