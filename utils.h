#ifndef _UTILS_H
#define _UTILS_H

#include <stdint.h>
#include <stddef.h>

typedef struct
{
    int encryption_flag;
    int key_length;
    char *input_file_path;
	char *output_file_path;
    char *key_file_path;
	char password[80];
    uint8_t key[32];
    uint8_t iv[16];

} Arguments;

void
print_hex_key(uint8_t *key);

void
hash_password(uint8_t *key,
              char *password,
              int len);

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

void
check(int assert_result, char *explanation);

void
convert_to_hex_string(char *out, 
                      uint8_t *in, 
                      size_t out_len, 
                      size_t in_len);

void
convert_to_uint8_array(uint8_t *out, 
                       char *in, 
                       size_t in_size);

#endif
