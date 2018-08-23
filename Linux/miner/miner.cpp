/**
 * 	TrustNote Equihash PoW Miner
 *
 *
 *	Copyright @ TrustNote.com
 *
 * 	@history
 *	Created by Liu QiXing on 2018/8/22.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "bitcoin-consensus-params.h"
#include "uint256.h"
#include "arith_uint256.h"
#include "equihash_avx2.h"
#include "crypto/blake2b.h"

#include "utility.h"
#include "miner.h"



/**
 *	main
 *
 *	@return	{int}
 */
int main( void )
{
	return 0;
}




/**
 *	start mining
 *
 *	@param	{uint8_t*}	putInputHeader		140 byte header
 *	@param	{uint32_t}	nDifficulty
 *	@param	{uint32_t}	nNonceStart		@default = 0
 *	@param	{uint32_t}	nNonceTimes		@default = 0
 *	@return	{uint32_t}
 */
int startMining( uint8_t * putInputHeader, uint32_t nDifficulty, uint32_t nNonceStart /* = 0 */, uint32_t nNonceTimes /* = 0 */ )
{
	uint32_t unRet;
	uint8_t utOutContext[ 32 ];
	char szHexHash[ 128 ];

	if ( NULL == putInputHeader )
	{
		return -1;
	}

	//	...
	unRet = -1;

	//	...
	void * pvContextAlloc	= malloc( TRUSTNOTE_MINER_CONTEXT_SIZE + 4096 );
	void * pvContext	= (void*)( ( (long)pvContextAlloc + 4095 ) & -4096 );
	void * pvContextEnd	= pvContext + TRUSTNOTE_MINER_CONTEXT_SIZE;

	//	...
	uint32_t uNonce		= nNonceStart;
	uint32_t nNonceEnd	= nNonceTimes > 0
					? ( nNonceStart + nNonceTimes ) >= UINT32_MAX
						? ( UINT32_MAX - nNonceStart - 1 )
						: ( nNonceStart + nNonceTimes )
					: ( UINT32_MAX - nNonceStart - 1 );
	for ( ; uNonce < nNonceEnd; uNonce ++ )
	{
		//
		//	calculate ...
		//
		EhPrepare( pvContext, (void *)putInputHeader );
		int32_t nSolutionCount = EhSolver( pvContext, uNonce );

		for ( int n = 0; n < nSolutionCount; n ++ )
		{
			//
			//	blake2b	512/256
			//	blake2s	256/128
			//
			blake2b( (uint8_t *)utOutContext, (uint8_t *)pvContext + n * 1344, NULL, sizeof(utOutContext), 1344, 0 );
			bytesToHexString( utOutContext, 32, szHexHash );

			//	filter
			int nCheck = filterDifficulty( szHexHash, nDifficulty );
			if ( 0 == nCheck )
			{
				//	printf( "[%d] - nonce: %d\t buff: %.*s\n", nCheck, uNonce, 64, szHexHash );
				unRet = 0;
				break;
			}

			//
			//	putInputHeader
			//	uNonce
			//	szHexHash
			//	nDifficulty
			//
		}
	}

	//	...
	free( pvContextAlloc );
	pvContextAlloc = NULL;

	//	...
	return unRet;
}


/**
 *	filter difficulty
 *	check proof of work
 *
 *	@param	{const char *}	pcszHash
 *	@param	{uint32_t}	nDifficulty
 *	@param	{const char *}	pcszPowLimit
 *	@return	{uint32_t}
 */
int filterDifficulty( const char * pcszHash, uint32_t nDifficulty, const char * pcszPowLimit /* = NULL */ )
{
	uint32_t unRet;
	bool fNegative;
	bool fOverflow;
	arith_uint256 bnTarget;

	if ( NULL == pcszHash )
	{
		return -2;
	}

	//	...
	unRet	= -1;

	//	...
	uint256 un256PowLimit	= uint256S( pcszPowLimit ? pcszPowLimit : TRUSTNOTE_MINER_POW_LIMIT );
	uint256 un256Hash	= uint256S( pcszHash );

	//	...
	bnTarget.SetCompact( nDifficulty, & fNegative, & fOverflow );

	//	check range
	if ( fNegative || 0 == bnTarget || fOverflow || bnTarget > UintToArith256( un256PowLimit ) )
	{
		//	printf("nBits below minimum work.\n");
		return -10;
	}

	//	check proof of work matches claimed amount
	if ( UintToArith256( un256Hash ) > bnTarget )
	{
		//	printf("hash doesn't match nBits.\n");
		return -20;
	}

	return unRet;
}



/**
 *	calculate next difficulty
 *
 *	@param	{uint32_t}	nDifficulty
 *	@param	{uint32_t}	nTimeUsed
 *	@param	{uint32_t}	nTimeStandard
 *	@param	{const char * }	pcszPowLimit
 *	@return	{uint32_t}
 */
int calculateNextDifficulty( uint32_t nDifficulty, uint32_t nTimeUsed, uint32_t nTimeStandard, const char * pcszPowLimit )
{
	uint32_t unRet;

	//	...
	unRet	= -1;


	return unRet;
}
uint64_t CalculateNextWorkRequired( uint32_t nDifficulty, uint32_t nTimeUsed, uint32_t nTimeStandard, const char * sPowLimit )
{
	uint256 powLimit = uint256S(sPowLimit);
	arith_uint256 bnTot {nDifficulty}, bnPowLimit;
	uint64_t nActualTmspan;
	int64_t MinActualTimespan = (nTimeStandard * ( 100 - 16 )) / 100;
	int64_t MaxActualTimespan = (nTimeStandard * ( 100 + 32 )) / 100;

	// 3/4 AveragingWindowTimespan + 1/4 nActualTimespan
	nActualTmspan = nTimeStandard + (nTimeUsed - nTimeStandard)/4;

	if (nActualTmspan < MinActualTimespan)	//	84% adjustment up
		nActualTmspan = MinActualTimespan;
	if (nActualTmspan > MaxActualTimespan)	// 	132% adjustment down
		nActualTmspan = MaxActualTimespan;

	// Retarget
	bnPowLimit = UintToArith256(powLimit);
	printf("powLimit: %d\n", bnPowLimit.GetCompact());

	arith_uint256 bnNew {bnTot};
	bnNew /= nTimeStandard;
	bnNew *= nActualTmspan;

	if (bnNew > bnPowLimit)
		bnNew = bnPowLimit;

	return bnNew.GetCompact();
}






