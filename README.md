국민대학교 암호 및 보안공학 연구실

AES, SHA256 구현

사용언어 : C

# easySHA256
A tiny and simple C library without dependencies that compute SHA256 digest from a file. Comes with a test program that compute the SHA256 disgest of an input file.

To use this library you need to add these files to your project: `prog_bar.c`,`prog_bar.h`, `sha256.c` and `sha256.h`. After that, just put `#include <sha256.h>` on you main program.

These are the functions available:
```
//Return 32 bytes digest of Data array on success. Return NULL if fail.
//VerboseStatus = SHA256_VERBOSE --> Will print progress
//VerboseStatus = SHA256_NOT_VERBOSE --> Will not print progress
uint8_t *sha256_data(uint8_t *Data, uint64_t DataSizeByte, uint8_t VerboseStatus);

//Return 32 bytes digest of file on success. Return NULL if fail.
//VerboseStatus = SHA256_VERBOSE --> Will print progress
//VerboseStatus = SHA256_NOT_VERBOSE --> Will not print progress
uint8_t *sha256_file(const char *Filename, uint8_t VerboseStatus);
```
