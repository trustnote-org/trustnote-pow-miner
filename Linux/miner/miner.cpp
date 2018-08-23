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
#include <algorithm>

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
	uint8_t utInputHeader[ 140 ];
	uint256 un256Difficulty		= uint256S( TRUSTNOTE_MINER_POW_LIMIT );
	arith_uint256 bn256Difficulty	= UintToArith256( un256Difficulty );
	uint32_t uDifficulty	= bn256Difficulty.GetCompact();
	uint32_t uNonceStart	= 0;
	uint32_t uNonceTimes	= 30000;
	uint32_t uNonce;
	char szHash[ 64 ];

	//	...
	startMining( utInputHeader, uDifficulty, uNonceStart, uNonceTimes, &uNonce, szHash );

	return 0;
}




/**
 *	start mining
 *
 *	@param	{uint8_t*}	putInputHeader		140 bytes header
 *	@param	{uint32_t}	uDifficulty
 *	@param	{uint32_t}	uNonceStart
 *	@param	{uint32_t}	uNonceTimes
 *	@param	{uint32_t *}	puNonce			OUT 4 bytes
 *	@param	{char *}	pcszHash		OUT 64 bytes
 *	@return	{uint32_t}
 */
int startMining( uint8_t * putInputHeader, uint32_t uDifficulty, uint32_t uNonceStart, uint32_t uNonceTimes, OUT uint32_t * puNonce, OUT char * pszHash )
{
	int nRet;
	uint8_t utOutContext[ 32 ];
	char szHexHash[ 128 ];

	if ( NULL == putInputHeader )
	{
		return -1000;
	}
	if ( NULL == puNonce )
	{
		return -1002;
	}
	if ( NULL == pszHash )
	{
		return -1003;
	}

	//	...
	nRet = -1;

	//	...
	void * pvContextAlloc	= malloc( TRUSTNOTE_MINER_CONTEXT_SIZE + 4096 );
	void * pvContext	= (void*)( ( (long)pvContextAlloc + 4095 ) & -4096 );
	void * pvContextEnd	= pvContext + TRUSTNOTE_MINER_CONTEXT_SIZE;

	//	...
	uint32_t uNonce		= uNonceStart;
	uint32_t nNonceEnd	= uNonceTimes > 0
					? ( uNonceStart + uNonceTimes ) >= UINT32_MAX
						? ( UINT32_MAX - uNonceStart - 1 )
						: ( uNonceStart + uNonceTimes )
					: ( UINT32_MAX - uNonceStart - 1 );
	for ( ; uNonce < nNonceEnd; uNonce ++ )
	{
		//
		//	calculate ...
		//
		EhPrepare( pvContext, (void *)putInputHeader );
		int32_t nSolutionCount = std::min( 3, EhSolver( pvContext, uNonce ) );

		for ( int n = 0; n < nSolutionCount; n ++ )
		{
			//
			//	blake2b	512/256
			//	blake2s	256/128
			//
			blake2b( (uint8_t *)utOutContext, (uint8_t *)pvContext + n * 1344, NULL, sizeof(utOutContext), 1344, 0 );
			bytesToHexString( utOutContext, 32, szHexHash );

			#ifdef _DEBUG
				printf( "Nonce: %d\t : %.*s\n", uNonce, 64, szHexHash );
			#endif

			//	filter
			int nCheck = filterDifficulty( uDifficulty, szHexHash );
			if ( 0 == nCheck )
			{
				#ifdef _DEBUG
					printf( ">>> Done!\n" );
				#endif

				//	printf( "[%d] - nonce: %d\t buff: %.*s\n", nCheck, uNonce, 64, szHexHash );
				nRet = 0;
				*puNonce = uNonce;
				memcpy( pszHash, szHexHash, 64 );

				//	...
				break;
			}
		}

		#ifdef _DEBUG
			printf( "\n" );
		#endif

		if ( 0 == nRet )
		{
			//	done
			break;
		}
	}

	//	...
	free( pvContextAlloc );
	pvContextAlloc = NULL;

	//	...
	return nRet;
}


/**
 *	stop mining
 *	@return	{int}
 */
int stopMining()
{
	return 0;
}


/**
 *	check proof of work
 *
 *	@param	{uint8_t *}	putInputHeader
 *	@param	{uint32_t}	uDifficulty
 *	@param	{uint32_t}	uNonce
 *	@param	{const char *}	pcszHash		with length 64
 *	@return	{int}
 *		0	- okay
 */
int checkProofOfWork( uint8_t * putInputHeader, uint32_t uDifficulty, uint32_t uNonce, const char * pcszHash )
{
	int nRet;

	if ( NULL == putInputHeader )
	{
		return -1000;
	}
	if ( NULL == pcszHash )
	{
		return -1001;
	}
	if ( 64 != strlen( pcszHash ) )
	{
		return -1002;
	}

	uint8_t utOutContext[ 32 ];
	char szHexHash[ 128 ];

	//	...
	nRet = -1;

	//	...
	void * pvContextAlloc	= malloc( TRUSTNOTE_MINER_CONTEXT_SIZE + 4096 );
	void * pvContext	= (void*)( ( (long)pvContextAlloc + 4095 ) & -4096 );
	void * pvContextEnd	= pvContext + TRUSTNOTE_MINER_CONTEXT_SIZE;

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
		blake2b( (uint8_t *)utOutContext, (uint8_t *)pvContext + n * 1344, NULL, sizeof( utOutContext ), 1344, 0 );
		bytesToHexString( utOutContext, 32, szHexHash );

		if ( 0 == strcasecmp( szHexHash, pcszHash ) )
		{
			//
			//	hex value matched
			//
			int nCheck = filterDifficulty( uDifficulty, szHexHash );
			if ( 0 == nCheck )
			{
				//
				//	difficulty filtered as okay
				//
				nRet = 0;
				break;
			}
		}
	}

	//	...
	free( pvContextAlloc );
	pvContextAlloc = NULL;

	return nRet;
}



/**
 *	filter difficulty
 *	check proof of work
 *
 *	@param	{const char *}	pcszHash
 *	@param	{uint32_t}	uDifficulty
 *	@param	{const char *}	pcszPowLimit
 *	@return	{uint32_t}
 */
int filterDifficulty( uint32_t uDifficulty, const char * pcszHash, const char * pcszPowLimit /* = NULL */ )
{
	bool fNegative;
	bool fOverflow;
	arith_uint256 bnTarget;

	if ( NULL == pcszHash )
	{
		return -1000;
	}

	//	...
	uint256 un256PowLimit	= uint256S( pcszPowLimit ? pcszPowLimit : TRUSTNOTE_MINER_POW_LIMIT );
	uint256 un256Hash	= uint256S( pcszHash );

	//	...
	bnTarget.SetCompact( uDifficulty, & fNegative, & fOverflow );

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

	return 0;
}


/**
 *	calculate next difficulty
 *
 *	@param	{uint32_t}	uDifficulty
 *	@param	{uint32_t}	uTimeUsed
 *	@param	{uint32_t}	uTimeStandard
 *	@param	{const char * }	pcszPowLimit
 *	@return	{uint32_t}
 */
uint32_t calculateNextDifficulty( uint32_t uDifficulty, uint32_t uTimeUsed, uint32_t uTimeStandard, const char * pcszPowLimit )
{
	uint256 powLimit		= uint256S( pcszPowLimit );
	arith_uint256 bnTot {uDifficulty}, bnPowLimit;
	uint64_t u64ActualTimeSpan;
	int64_t n64MinActualTimeSpan	= ( uTimeStandard * ( 100 - 16 ) ) / 100;
	int64_t n64MaxActualTimeSpan	= ( uTimeStandard * ( 100 + 32 ) ) / 100;

	//	3/4 AveragingWindowTimespan + 1/4 nActualTimespan
	u64ActualTimeSpan	= uTimeStandard + ( uTimeUsed - uTimeStandard ) / 4;

	if ( u64ActualTimeSpan < n64MinActualTimeSpan )
	{
		//	84% adjustment up
		u64ActualTimeSpan = n64MinActualTimeSpan;
	}
	if ( u64ActualTimeSpan > n64MaxActualTimeSpan )
	{
		// 	132% adjustment down
		u64ActualTimeSpan = n64MaxActualTimeSpan;
	}

	//
	//	retarget
	//
	bnPowLimit	= UintToArith256( powLimit );
	//printf("powLimit: %d\n", bnPowLimit.GetCompact());

	arith_uint256 bnNew {bnTot};
	bnNew	/= uTimeStandard;
	bnNew	*= u64ActualTimeSpan;

	if ( bnNew > bnPowLimit )
	{
		bnNew = bnPowLimit;
	}

	return bnNew.GetCompact();
}


