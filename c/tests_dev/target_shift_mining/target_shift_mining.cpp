//
// Created by Liu QiXing on 2018/11/02.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <sys/time.h>

#include "uint256.h"
#include "arith_uint256.h"
#include "miner.h"



#ifndef IN
	#define IN
#endif
#ifndef OUT
	#define OUT
#endif


void test_target_shift_mining();
void test_start_mining( uint32_t uBits );


/**
 *	@private
 */
static int g_nShift	= 0;





int main( int argc, char **argv )
{
	if ( 2 != argc )
	{
		printf( "Usage:\n$%s {shift number}\n\n\n", argv[ 0 ] );
		return -1;
	}

	//	...
	g_nShift	= atoi( argv[ 1 ] );
	if ( g_nShift < 0 )
	{
		g_nShift = 0;
	}

	printf( "==> will mine with bits by shift right %d\n", g_nShift );

	//	...
	test_target_shift_mining();

	//	...
	return 0;
}




void test_target_shift_mining()
{
	arith_uint256 bnNormalUInt256;
	char szTarget[ 66 ];
	uint32_t uBits;

	//
	//	shift right
	//
	bnNormalUInt256	= UintToArith256( uint256S( TRUSTNOTE_MINER_POW_LIMIT_TARGET ) );

	//	...
	bnNormalUInt256 >>= g_nShift;

	//	...
	uBits	= bnNormalUInt256.GetCompact();

	memset( szTarget, 0, sizeof( szTarget ) );
	getTargetByBits( uBits, szTarget, sizeof( szTarget ) );

	printf( "==> bits 0x%08X, ", uBits );
	printf( "%*d, ", 10, uBits );
	printf( "%s\n", szTarget );

	//	...
	test_start_mining( uBits );
}

void test_start_mining( uint32_t uBits )
{
	uint8_t utInputHeader[ 140 ];
	uint32_t uNonceStart		= 0;
	uint32_t uCalcTimes		= 30000000000;
	uint32_t uNonce;
	char szHexHash[ 64 ];
	int nCheckPoW;
	struct timeval tval_before, tval_after, tval_result;

	//	...
	gettimeofday( &tval_before, NULL );

	for ( int n = 0; n < 3; n ++ )
	{
		printf( "==> round %d\n", n + 1 );

		//	...
		memset( utInputHeader, 0, sizeof( utInputHeader ) );
		memset( szHexHash, 0, sizeof( szHexHash ) );
		for ( int i = 0; i < 140; i ++ )
		{
			utInputHeader[ i ] = n + i;
		}
		int nCallStartMining = startMining( utInputHeader, uBits, uNonceStart, uCalcTimes, &uNonce, szHexHash, sizeof( szHexHash ) );
		printf(
			"==> call=%d, nonce: %u, hex: %.*s\n",
			nCallStartMining,
			uNonce,
			64,
			szHexHash
		);

		//	...
		nCheckPoW = checkProofOfWork( utInputHeader, uBits, uNonce, szHexHash );
		printf( "==> checkProofOfWork = %d : %s\n",
			nCheckPoW,
			0 == nCheckPoW ? "Winer Winner Chicken Dinner!" : "Not Okay!" );
	}

	gettimeofday( &tval_after, NULL );
	timersub( &tval_after, &tval_before, &tval_result );

	printf( "==> Time elapsed: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec );

}


