//compile with
//gcc -o quickbench quickbench.c equihash_avx2.o
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include <sys/sysinfo.h>

#include "threadpool.h"

using namespace std;

#define ITERATIONS 		10

#define THREADS 8
#define QUEUE	64

#define TRUE 	1
#define FALSE 	0
#define ERROR 	-1

typedef unsigned int          uint32_t;
typedef unsigned char         uint8_t;
typedef signed int            int32_t;

pthread_mutex_t lock;
int done = 0;

struct Cache{
	void *head_alloc;
	void *context;
	uint32_t workers;
	uint32_t index;
	uint32_t nonce;
	pthread_mutex_t lock;
};


//void EhPrepare(void *context, void *input);
//int32_t EhSolver(void *context, uint32_t nonce);


void thread_call(void *arg){
	
	Cache *cache_p = (Cache*)arg;
	struct timespec time0, time1;
	
	for (int i = 0; i < ITERATIONS; i++) {
		clock_gettime(CLOCK_MONOTONIC, &time0);
		int numsolutions = 0;//EhSolver(cache_p->context, cache_p->nonce);
		clock_gettime(CLOCK_MONOTONIC, &time1);
		cache_p->nonce += cache_p->workers;
		uint32_t delta_time = (uint32_t) ((time1.tv_sec * 1000000000 + time1.tv_nsec)
				- (time0.tv_sec * 1000000000 + time0.tv_nsec))/1000000;
		printf("zhe number of thread is %d, Time: %u ms, solutions: %u\n",
			cache_p->nonce % cache_p->workers,
			delta_time,
			numsolutions);

	}
}

int init_threads_cache(map<int, Cache> threads_cache, void *inputheader){

	if (threads_cache == NULL || inputheader == NULL)
		return FALSE;

	for(int i = 0;i < THREADS; i++){
		Cache cache;
		cache.head_alloc = malloc(TRUSTNOTE_MINER_CONTEXT_SIZE+4096);
		cache.context = (void*) (((long) cache.head_alloc+4095) & -4096);

		//EhPrepare(cache.context, inputheader);
		cache.workers, cache.nonce = THREADS, i;
		
		pthread_mutex_init(&(cache.lock), NULL);
		
		(*threads_cache)[i] = cache;
	}
	return TRUE;
}

int free_threads_cache(map<int,Cache> threads_cache){

	if (threads_cache == NULL)
			return FALSE;

	for(int i = 0;i < THREADS; i++){
		if ((*threads_cache)[i].head_alloc != NULL)
			free((*threads_cache)[i].head_alloc);
		pthread_mutex_destroy(&((*threads_cache)[i].lock));
	}
	
	return TRUE;
}

int main(int argc, char** argv)
{
	threadpool_t *pool;
	uint8_t inputheader[144];	//140 byte header
	map<int, Cache> threads_cache;

	FILE *infile = 0;
	infile = fopen("input.bin", "rb");
	if (infile) {
		puts("Reading input.bin");
		fread(inputheader, 140, 1, infile);
		fclose(infile);
	} else {
		return -1;
	}

	// init data of threads pool
	if (init_threads_cache(threads_cache, (void*)inputheader) == FALSE)
		fprintf(stderr, "init handle cache error !\n");

	if(free_threads_cache(threads_cache) == FALSE)
		fprintf(stderr, "free threads cache error!\n");
	
	return 0;
}
