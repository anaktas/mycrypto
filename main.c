#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "utils.h"

int main(int argc, char *argv[]) {
    char *output;

    if (argc == 2) {
        char *file_path = argv[1];
        long int file_size = 0;

        file_size = get_file_size(file_path);
        if (file_size == 0){
            file_size = 16;
        }

        output = (char*) malloc(sizeof(char) * file_size);
        
        int result = read_file(file_path, output, file_size);
        printf("The output is:\n%s\n", output);
    }

    free(output);

    return 0;
}