#ifndef _UTILS_H
#define _UTILS_H

int read_file(char *file_path, char *output, long int output_size);
int write_file(char *output, char *file_path);
long int get_file_size(char *file_path);

#endif