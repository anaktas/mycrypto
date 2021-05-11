#ifndef _AES_H
#define _AES_H

#include <stdint.h>
#include <stddef.h>

typedef enum {
    NO_ERROR = 0,
    ERROR_INVALID_PARAMETER = 1,
    ERROR_INVALID_KEY_LENGTH = 2
} Aes_Error;

typedef enum {
    KEY_128 = 0,
    KEY_192 = 1,
    KEY_256 = 2
} Aes_key_size;

typedef struct {
    Aes_key_size key_size;
    uint8_t key;
    uint8_t number_of_rounds;
    uint32_t encryption_scheduled_key[60];
    uint32_t decryption_scheduled_key[60];
} Aes;

/**
 * AES functions for key expansion, encryption and decryption
 */
Aes_Error init(Aes *aes, const uint8_t *key, size_t key_length);
void encrypt(Aes *aes, const uint8_t *input, uint8_t *output);
void decrypt(Aes *aes, const uint8_t *input, uint8_t *output);

/**
 * Helpers
 */
static inline __attribute__((always_inline)) 
uint32_t load_32_le(const uint8_t *key);
static inline __attribute__((always_inline)) 
uint32_t rol_32(uint32_t a, uint32_t n);
static inline __attribute__((always_inline)) 
uint32_t store_32_le(uint32_t a, const uint8_t *p);

#endif