#ifndef _AES_H
#define _AES_H

#include <stdint.h>
#include <stddef.h>

/**
 * Rotate left operation macro (32-bit)
 */
#define ROL32(a, n) (((a) << (n)) | ((a) >> (32 - (n))))

/**
 * Store unaligned 32-bit integer macro (little-endian encoding)
 */
#define STORE32LE(a, p) \
    ((uint8_t *)(p))[0] = ((uint32_t)(a) >> 0) & 0xFFU, \
    ((uint8_t *)(p))[1] = ((uint32_t)(a) >> 8) & 0xFFU, \
    ((uint8_t *)(p))[2] = ((uint32_t)(a) >> 16) & 0xFFU, \
    ((uint8_t *)(p))[3] = ((uint32_t)(a) >> 24) & 0xFFU

/**
 * Load unaligned 32-bit integer macro (little-endian encoding)
 */
#define LOAD32LE(p) ( \
    ((uint32_t)(((uint8_t *)(p))[0]) << 0) | \
    ((uint32_t)(((uint8_t *)(p))[1]) << 8) | \
    ((uint32_t)(((uint8_t *)(p))[2]) << 16) | \
    ((uint32_t)(((uint8_t *)(p))[3]) << 24))

typedef enum
{
    NO_ERROR = 0,
    ERROR_INVALID_PARAMETER = 1,
    ERROR_INVALID_KEY_LENGTH = 2
} Aes_Error;

typedef enum
{
    KEY_128 = 0,
    KEY_192 = 1,
    KEY_256 = 2
} Aes_key_size;

typedef struct
{
    Aes_key_size key_size;
    uint8_t key;
    uint8_t number_of_rounds;
    uint32_t encryption_scheduled_key[60];
    uint32_t decryption_scheduled_key[60];
} Aes;

/**
 * AES functions for key expansion, encryption and decryption
 */
Aes_Error
init(Aes *aes, const uint8_t *key, size_t key_length);

void
encrypt(Aes *aes, const uint8_t *input, uint8_t *output);

void
decrypt(Aes *aes, const uint8_t *input, uint8_t *output);

/**
 * Helpers
 */
static inline __attribute__((always_inline)) uint32_t
load_32_le(const uint8_t *key);

static inline __attribute__((always_inline)) uint32_t
rol_32(uint32_t a, uint32_t n);

static inline __attribute__((always_inline)) void
store_32_le(uint32_t a, const uint8_t *p);

#endif