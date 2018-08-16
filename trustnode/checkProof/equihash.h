#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define CONTEXT_SIZE 178033152
#define ITERATIONS 10

#ifdef __cplusplus
extern "C" {
#endif

int checkProofOfWork(const char* hash, int nBits, const char* powLimit);

#ifdef __cplusplus
}
#endif
