#ifndef _UTILS_H
#define _UTILS_H

typedef struct
{
    int encryption_flag;
    char *input_file_path;
	char *output_file_path;
    char *key_file_path;
    int key_length;
} Arguments;

int
read_file(char *file_path,
          char *output,
          long int output_size);

int
write_file(char *output, char *file_path);

long int
get_file_size(char *file_path);

int
parse_arguments(Arguments *args,
                int argc,
                char *argv[]);

#endif
