//compile with
//gcc -o quickbench quickbench.c equihash_avx2.o
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "../includes/equihash_avx2.h"
#include "crypto/blake2b.h"
#include "proof/proof.h"

#define CONTEXT_SIZE	178033152
#define ITERATIONS	10


extern char testinput[];



void ByteToHexStr( const uint8_t * source, char * dest, int sourceLen )
{
    short i;
    unsigned char highByte, lowByte;
 
    for (i = 0; i < sourceLen; i++)
    {
        highByte = source[i] >> 4;
        lowByte = source[i] & 0x0f ;

        if (highByte > 0x09)
                dest[i * 2] = highByte + 0x57;
        else
                dest[i * 2] = highByte | 0x30;
 
        if (lowByte > 0x09)
            dest[i * 2 + 1] = lowByte + 0x57;
        else
            dest[i * 2 + 1] = lowByte | 0x30;
    }
    return;
}


//
//int fulltest(uint8_t* context, char* target, int conLen){
//
//	unsigned char highByte, lowByte;
//	char buff[2];
//
//	for(int i = 0;i < conLen; i++){
//
//		highByte = context[i] >> 4;
//		lowByte = context[i] & 0x0f;
//
//		if (highByte > 0x09)
//                buff[0] = highByte + 0x57;
//        else
//                buff[0] = highByte | 0x30;
//		if(buff[0] != target[i*2])
//			return -1;
//
//        if (lowByte > 0x09)
//            buff[1] = lowByte + 0x57;
//        else
//            buff[1] = lowByte | 0x30;
//		if(buff[1] != target[i*2+1])
//			return -1;
//
//	}
//
//	return 0;
//}
//int equihash( uint8_t * input, uint32_t nonce, char * target, int inputlen )
//{
//	void * context_alloc;
//	void * context;
//	char * pTarget = target;
//	uint8_t outctx[ 32 ];
//	char buff[ 64 ];
//	int err = -1;
//
//	//	...
//	if ( 140 != inputlen  )
//	{
//		return -1;
//	}
//
//	//	...
//	context_alloc	= malloc( CONTEXT_SIZE + 4096 );
//	context		= (void*)( ( (long)context_alloc + 4095 ) & -4096 );
//
//	if ( ! strncasecmp( "0x", target, 2 ) )
//	{
//		pTarget = & target[ 2 ];
//	}
//
//	//	...
//	EhPrepare( context, input );
//	int32_t numsolutions = EhSolver(context, nonce);
//	printf("numsolutions = %d\n", numsolutions);
//	for ( int i = 0; i < numsolutions; i++ )
//	{
//		blake2b( (uint8_t*)outctx, (uint8_t*)context+i*1344, NULL, sizeof(outctx), 1344, 0 );
//		if ( ! fulltest( outctx, pTarget, 32 ) )
//		{
//			ByteToHexStr( outctx, buff, 32 );
//			printf( "target = %.*s\n\n", 64, buff );
//			err = 0;
//			break;
//		}
//	}
//
//	free( context_alloc );
//	context_alloc = NULL;
//
//	//	...
//	return err;
//}


int main( void )
{
	FILE * fp;
	uint8_t utInputHeader[ 140 ];	//	140 byte header
	char szBuff[ 280 ];
	void * pvContextAlloc;
	void * pvContext;
	void * pvContextEnd;
	int32_t nSolutionCount;
	uint32_t nNonce;
	uint8_t utOutContext[ 32 ];

	//	...
	fp = fopen( "input.bin", "rb" );
	if ( ! fp )
	{
		puts( "input.bin not found, use sample data (beta1 testnet block 2)" );
		return -1;
	}

	//	...
	puts( "Reading input.bin" );
	fread( utInputHeader, 140, 1, fp );
	fclose( fp );

	//	...
	pvContextAlloc	= malloc( CONTEXT_SIZE + 4096 );
	pvContext	= (void*)( ( (long)pvContextAlloc + 4095 ) & -4096 );
	pvContextEnd	= pvContext + CONTEXT_SIZE;

	ByteToHexStr( utInputHeader, szBuff, 140 );
	printf( "input: %.*s\n", 280, szBuff );

	//	...
	nNonce = 0;
	for ( int i = 0; i < 10000000; i ++ )
	{
		EhPrepare( pvContext, (void *)utInputHeader );
		nSolutionCount	= EhSolver( pvContext, nNonce );
		for ( int n = 0; n < nSolutionCount; n ++ )
		{
			blake2b( (uint8_t*)utOutContext, (uint8_t*)pvContext + n * 1344, NULL, sizeof(utOutContext), 1344, 0 );
			ByteToHexStr( utOutContext, szBuff, 32 );

			int nCheck = checkProofOfWork
				(
					szBuff,
					1000,
					"007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
				);
			printf( "[%d] - nonce: %d\t buff: %.*s\n", nCheck, nNonce, 64, szBuff );
			if ( 0 == nCheck )
			{
				printf( "Find it out." );
				break;
			}
		}

		nNonce ++;
	}

	free( pvContextAlloc );
	pvContextAlloc = NULL;

	//	...
	return 0;
}
