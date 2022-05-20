# mycrypto
An example of the Rijndael (AES) encryption/decryption implementation in C .

# Build

## Debug

```
make clean
make debug
```

## Release

```
make clean
mame release
```

*Note:* Please make sure you comment line 4 in the `config.h` file. This will be automated in
some future commit.

# Usage

```
./aes [-e|-d] -i <INPUT_FILE_PATH> -o <OUTPUT_FILE_PATH> -l [128|192|256]
    -e  Encryption flag.
    -d  Decryption flag.
    -i  Input file path. A valid file path should follow this flag.
    -o  Output file path. A valid file path should follow this flag.
    -l  Encryption key length. Available options: 128, 192 and 256.
```
