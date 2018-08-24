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
	uint256 un256Difficulty		= uint256S( "00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff" );
	arith_uint256 bn256Difficulty	= UintToArith256( un256Difficulty );
	uint32_t uDifficulty	= bn256Difficulty.GetCompact();
	uint32_t uNonceStart	= 0;
	uint32_t uNonceTimes	= 30000;
	uint32_t uNonce;
	char szHexHash[ 64 ];
	int nCheckPoW;


	uint32_t uNewDifficulty1	= calculateNextDifficulty( uDifficulty, 15000, 15000 );
	uint32_t uNewDifficulty2	= calculateNextDifficulty( uDifficulty, 30000, 15000 );
	uint32_t uNewDifficulty3	= calculateNextDifficulty( uDifficulty, 10000, 15000 );
	uint32_t uNewDifficulty4	= calculateNextDifficulty( uDifficulty, 40000, 15000 );
	uint32_t uNewDifficulty5	= calculateNextDifficulty( uDifficulty, 50000, 15000 );
	uint32_t uNewDifficulty6	= calculateNextDifficulty( uDifficulty, 60000, 15000 );
	uint32_t uNewDifficulty7	= calculateNextDifficulty( uDifficulty, 70000, 15000 );
	uint32_t uNewDifficulty8	= calculateNextDifficulty( uDifficulty, 80000, 15000 );
	uint32_t uNewDifficulty9	= calculateNextDifficulty( uDifficulty, 90000, 15000 );


	#ifdef _DEBUG
		printf( "### : will start mining with difficulty : %u\n", uDifficulty );
	#endif

	//	...
	memset( utInputHeader, 0, sizeof( utInputHeader ) );
	startMining( utInputHeader, uDifficulty, uNonceStart, uNonceTimes, &uNonce, szHexHash, sizeof( szHexHash ) );
	#ifdef _DEBUG
		printf( "### : %u\t : %.*s\n", uNonce, 64, szHexHash );
	#endif
	//	...
	nCheckPoW = checkProofOfWork( utInputHeader, uDifficulty, uNonce, szHexHash );
	#ifdef _DEBUG
		printf( "checkProofOfWork : %s\n\n\n\n", 0 == nCheckPoW ? "Winer Winner Chicken Dinner!" : "Not Okay!" );
	#endif

	//	...
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
 *	@param	{char *}	pszHashHex		OUT 64 bytes
*	@param	{uint32_t}	uHashHexLength
 *	@return	{uint32_t}
 */
int startMining(
	uint8_t * putInputHeader,
	uint32_t uDifficulty,
	uint32_t uNonceStart,
	uint32_t uNonceTimes,
	OUT uint32_t * puNonce,
	OUT char * pszHashHex,
	uint32_t uHashHexLength )
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
	if ( NULL == pszHashHex )
	{
		return -1003;
	}
	if ( 64 != uHashHexLength )
	{
		return -1004;
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
				printf( "Nonce: %u\t : %.*s\n", uNonce, 64, szHexHash );
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
				memcpy( pszHashHex, szHexHash, std::min( (uint32_t)64, uHashHexLength ) );

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
int checkProofOfWork(
	uint8_t * putInputHeader,
	uint32_t uDifficulty,
	uint32_t uNonce,
	const char * pcszHashHex )
{
	int nRet;

	if ( NULL == putInputHeader )
	{
		return -1000;
	}
	if ( NULL == pcszHashHex )
	{
		return -1001;
	}
	if ( strlen( pcszHashHex ) < 64 )
	{
		#ifdef _DEBUG
			printf( "checkProofOfWork :: pcszHashHex : %s, length : %u\n", pcszHashHex, strlen( pcszHashHex ) );
		#endif
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

		if ( 0 == strncasecmp( szHexHash, pcszHashHex, 64 ) )
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
 *	@param	{uint32_t}	uDifficulty
 *	@param	{const char *}	pcszHashHex
 *	@return	{uint32_t}
 */
int filterDifficulty(
	uint32_t uDifficulty,
	const char * pcszHashHex )
{
	bool fNegative;
	bool fOverflow;
	arith_uint256 bnTarget;

	if ( NULL == pcszHashHex )
	{
		return -1000;
	}
	if ( strlen( pcszHashHex ) < 64 )
	{
		#ifdef _DEBUG
			printf( "filterDifficulty :: pcszHashHex : %s, length : %u\n", pcszHashHex, strlen( pcszHashHex ) );
		#endif
		return -1002;
	}

	//	...
	uint256 un256PowLimit	= uint256S( TRUSTNOTE_MINER_POW_LIMIT );
	uint256 un256Hash	= uint256S( pcszHashHex );

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
 *	@param	{uint32_t}	uPreviousDifficulty
 *	@param	{uint32_t}	uTimeUsed
 *	@param	{uint32_t}	uTimeStandard
 *	@return	{uint32_t}
 */
uint32_t calculateNextDifficulty(
	uint32_t uPreviousDifficulty,
	uint32_t uTimeUsed,
	uint32_t uTimeStandard )
{
	uint32_t uRet			= -1;
	uint256 powLimit		= uint256S( TRUSTNOTE_MINER_POW_LIMIT );
	arith_uint256 bnTot, bnPowLimit;
	uint64_t u64ActualTimeSpan;
	int64_t n64MinActualTimeSpan	= ( uTimeStandard * ( 100 - 16 ) ) / 100;
	int64_t n64MaxActualTimeSpan	= ( uTimeStandard * ( 100 + 32 ) ) / 100;

	//	3/4 uTimeStandard + 1/4 uTimeUsed
	u64ActualTimeSpan	= ( uTimeStandard * 3 + uTimeUsed ) / 4;

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

	//	...
	bnTot.SetCompact( uPreviousDifficulty );

	//	...
	arith_uint256 bnNew {bnTot};
	#ifdef _DEBUG
		printf( "*** before\t:\t%u\n", bnNew.GetCompact() );
		printf( "*** u64ActualTimeSpan\t:\t%u\n", u64ActualTimeSpan );
	#endif

	bnNew	/= uTimeStandard;
	bnNew	*= u64ActualTimeSpan;

	#ifdef _DEBUG
		printf( "*** after\t:\t%u\n", bnNew.GetCompact() );
	#endif

	//	限制不要太简单了
	if ( bnNew > bnPowLimit )
	{
		bnNew = bnPowLimit;
	}

	//	...
	uRet = bnNew.GetCompact();
	#ifdef _DEBUG
		printf( "calculateNextDifficulty :: uPreviousDifficulty : %u, uTimeUsed : %u, uTimeStandard : %u, return : %u, %s\n",
			uPreviousDifficulty, uTimeUsed, uTimeStandard, uRet, bnNew.ToString() );
	#endif

	return uRet;
}


