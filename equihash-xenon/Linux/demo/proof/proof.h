#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

int checkProofOfWork(const char* hash, int nBits, const char* powLimit);

#ifdef __cplusplus
}
#endif
