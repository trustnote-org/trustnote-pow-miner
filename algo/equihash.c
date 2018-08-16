#include <miner.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include <crypto/blake2b.h>

#define CONTEXT_SIZE 178033152
#define ITERATIONS 10

//Linkage with assembly
//EhPrepare takes in 136 bytes of input. The remaining 4 bytes of input is fed as nonce to EhSolver.
//EhPrepare saves the 136 bytes in context, and EhSolver can be called repeatedly with different nonce.
void EhPrepare(void *context, void *input);
int32_t EhSolver(void *context, uint32_t nonce);

int equihash(void* context, const unsigned char* input, uint32_t nonce){

	EhPrepare(context, (void *) input);
	int num = EhSolver(context, nonce);
	return num;
}

int scanhash_equihash(int thr_id, struct work *work, uint32_t max_nonce, uint64_t *hashes_done){

	void *context_alloc, *context;
	uint8_t *pInput = (uint8_t*)work->data;
	uint8_t hash[32];
	char buff[64];

	uint32_t nonce = work->nonce;
	volatile uint8_t *restart = &(work_restart[thr_id].restart);

	context_alloc = malloc(CONTEXT_SIZE+4096);
	context = (void*) (((long) context_alloc+4095) & -4096);

	do {
		int numsolutions = equihash(context, pInput, nonce);
		
		for (int i = 0;i < numsolutions; i++){
			blake2b(hash, (uint8_t*)context+i*1344, NULL, sizeof(uint8_t) * 32, 1344, 0);

			ByteToHexStr(hash, buff, 32);
			//printf("nonce = %d\t hash = %.*s\n\n", nonce, 64, buff);
			
			if (!checkProofOfWork(hash,
								 work->difficulty,
								 "0007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff")) 
			{
				printf("nonce = %d\t target = %.*s\n\n", nonce, 64, buff);
				memcpy(work->hash, hash, sizeof(work->hash));

				work_set_target_ratio(work, work->hash);
				*hashes_done = nonce - work->nonce;
				goto OUT;
			}
		}
		nonce++;

	} while (nonce < max_nonce && !(*restart));


	free(context_alloc);
	return -1;

OUT:
	free(context_alloc);

	return 0;
}




