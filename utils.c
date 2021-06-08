#include <stdio.h>
#include <stdlib.h>
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
    
    if (file_path == NULL) {
        file_path = "cipheroutput";
    }

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
    printf("fteel size: %ld\n", file_size);

    fclose(file);

    return file_size;
}