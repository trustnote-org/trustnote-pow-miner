/**
 * 	TrustNote Equihash PoW Miner
 *
 *
 *	Copyright @ TrustNote.org
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
#include <iostream>
#include <cstdlib>

#include "bitcoin-consensus-params.h"
#include "uint256.h"
#include "arith_uint256.h"
#include "equihash_avx2.h"
#include "crypto/blake2b.h"

#include "utility.h"
#include "trustnote-miner-deposit.h"
#include "trustnote-difficulty-bomb.h"
#include "miner.h"



#ifdef WIN32
	#ifndef strncasecmp
		#define strncasecmp strnicmp
	#endif
#endif



/**
 *	@private
 */
static bool _isEasierThanLimitByArithUInt256( const arith_uint256 & clsArithUInt256 );




/**
 *	main
 *
 *	@return	{int}
 */
#if ! defined( _TEST ) && ! defined( _LIB )
int main( void )
{
	return 0;
}
#endif




/**
 *	start mining
 *
 *	@param	{uint8_t*}	pcutInputHeader		140 bytes header
 *	@param	{uint32_t}	uBits
 *	@param	{uint32_t}	uNonceStart
 *	@param	{uint32_t}	uCalcTimes
 *	@param	{uint32_t *}	puNonce			OUT 4 bytes
 *	@param	{char *}	pszHashHex		OUT 64 bytes
*	@param	{uint32_t}	uHashHexLength
 *	@return	{uint32_t}
 */
EXPORT_API int startMining(
	const uint8_t * pcutInputHeader,
	const uint32_t uBits,
	const uint32_t uNonceStart,
	const uint32_t uCalcTimes,
	OUT uint32_t * puNonce,
	OUT char * pszHashHex,
	uint32_t uHashHexLength )
{
	int nRet;
	uint8_t utOutContext[ 32 ];
	char szHexHash[ 128 ];

	if ( NULL == pcutInputHeader )
	{
		#if defined( _DEBUG ) || defined( _TEST )
			printf( "startMining :: invalid parameter pcutInputHeader, NULL" );
		#endif
		return -1000;
	}
	if ( NULL == puNonce )
	{
		#if defined( _DEBUG ) || defined( _TEST )
			printf( "startMining :: invalid parameter puNonce, NULL" );
		#endif
		return -1002;
	}
	if ( NULL == pszHashHex )
	{
		#if defined( _DEBUG ) || defined( _TEST )
			printf( "startMining :: invalid parameter pszHashHex, NULL" );
		#endif
		return -1003;
	}
	if ( 64 != uHashHexLength )
	{
		#if defined( _DEBUG ) || defined( _TEST )
			printf( "startMining :: invalid parameter uHashHexLength, not 64" );
		#endif
		return -1004;
	}

	//	...
	nRet = -1;

	//	...
	void * pvContextAlloc	= malloc( TRUSTNOTE_MINER_CONTEXT_SIZE + 4096 );
	void * pvContext	= (void*)( ( (long long)pvContextAlloc + 4095 ) & -4096 );
	void * pvContextEnd	= (char*)pvContext + TRUSTNOTE_MINER_CONTEXT_SIZE;

	#if ! defined( _NO_CACHE_OPTIMIZE )
		void * pvCacheContextAlloc	= malloc( TRUSTNOTE_MINER_CONTEXT_SIZE + 4096 );
		void * pvCacheContext		= (void*)( ( (long long)pvCacheContextAlloc + 4095 ) & -4096 );
		void * pvCacheContextEnd	= (char*)pvCacheContext + TRUSTNOTE_MINER_CONTEXT_SIZE;

		EhPrepare( pvCacheContext, (void *)pcutInputHeader );
	#endif

	//	...
	uint32_t uNonce		= uNonceStart;
	uint32_t nNonceEnd	= uCalcTimes > 0
					? ( uNonceStart + uCalcTimes ) >= UINT32_MAX
						? ( UINT32_MAX - uNonceStart - 1 )
						: ( uNonceStart + uCalcTimes )
					: ( UINT32_MAX - uNonceStart - 1 );
	for ( ; uNonce < nNonceEnd; uNonce ++ )
	{
		#if defined( _DEBUG ) || defined( _TEST )
			printf( "startMining :: Nonce: %u\n", uNonce );
		#endif

		//
		//	prepare ...
		//
		#if ! defined( _NO_CACHE_OPTIMIZE )

			//	optimized by copying from memory
			memcpy( pvContext, pvCacheContext, TRUSTNOTE_MINER_CONTEXT_SIZE );
		#else
			//	prepare again
			EhPrepare( pvContext, (void *)pcutInputHeader );
		#endif


		#if defined( _DEBUG ) || defined( _TEST )
			printf( "startMining :: EhPrepare done\n" );
		#endif

		int32_t nSolutionCount = EhSolver( pvContext, uNonce );
		#if defined( _DEBUG ) || defined( _TEST )
			printf( "startMining :: EhSolver, nSolutionCount : %d\n", nSolutionCount );
		#endif
		nSolutionCount = std::min( 3, nSolutionCount );

		for ( int n = 0; n < nSolutionCount; n ++ )
		{
			//
			//	blake2b	512/256
			//	blake2s	256/128
			//
			blake2b( (uint8_t *)utOutContext, (uint8_t *)pvContext + n * 1344, NULL, sizeof(utOutContext), 1344, 0 );
			bytesToHexString( utOutContext, 32, szHexHash );

			#if defined( _DEBUG ) || defined( _TEST )
				printf( "startMining :: Nonce: %u\t : %.*s\n", uNonce, 64, szHexHash );
				printf( "startMining :: will filterDifficulty, uBits: %u\n", uBits );
			#endif

			//	filter
			int nCheck = filterDifficulty( uBits, szHexHash );

			#if defined( _DEBUG ) || defined( _TEST )
				printf( "startMining :: return by filterDifficulty: %d\n", nCheck );
			#endif

			if ( 0 == nCheck )
			{
				#if defined( _DEBUG ) || defined( _TEST )
					printf( "startMining :: >>> Done!\n" );
				#endif

				//	printf( "[%d] - nonce: %d\t buff: %.*s\n", nCheck, uNonce, 64, szHexHash );
				nRet = 0;
				*puNonce = uNonce;
				memcpy( pszHashHex, szHexHash, std::min( (uint32_t)64, uHashHexLength ) );

				//	...
				break;
			}
		}

		#if defined( _DEBUG ) || defined( _TEST )
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

	#if ! defined( _NO_CACHE_OPTIMIZE )
		free( pvCacheContextAlloc );
		pvCacheContextAlloc = NULL;
	#endif

	#if defined( _DEBUG ) || defined( _TEST )
		printf( "startMining :: ****** return value : %d\n", nRet );
	#endif


	//	...
	return nRet;
}


/**
 *	stop mining
 *	@return	{int}
 */
EXPORT_API int stopMining()
{
	return 0;
}


/**
 *	check proof of work
 *
 *	@param	{uint8_t *}	pcutInputHeader
 *	@param	{uint32_t}	uBits
 *	@param	{uint32_t}	uNonce
 *	@param	{const char *}	pcszHash		with length 64
 *	@return	{int}
 *		0	- okay
 */
EXPORT_API int checkProofOfWork(
	const uint8_t * pcutInputHeader,
	const uint32_t uBits,
	const uint32_t uNonce,
	const char * pcszHashHex )
{
	int nRet;

	if ( NULL == pcutInputHeader )
	{
		#ifdef _DEBUG
			printf( "checkProofOfWork :: p01 invalid pcutInputHeader\n" );
		#endif
		return -1000;
	}
	if ( NULL == pcszHashHex )
	{
		#ifdef _DEBUG
			printf( "checkProofOfWork :: p02 invalid pcszHashHex\n" );
		#endif
		return -1001;
	}
	if ( strlen( pcszHashHex ) < 64 )
	{
		#ifdef _DEBUG
			printf( "checkProofOfWork :: p03 pcszHashHex : %s, length : %zu\n", pcszHashHex, strlen( pcszHashHex ) );
		#endif
		return -1002;
	}

	uint8_t utOutContext[ 32 ];
	char szHashHex[ 128 ];

	//	...
	nRet = -1;

	try
	{
		#ifdef _DEBUG
			printf( "checkProofOfWork :: 001 will allocate memory\n" );
		#endif

		//	...
		void * pvContextAlloc	= malloc( TRUSTNOTE_MINER_CONTEXT_SIZE + 4096 );
		#ifdef _DEBUG
			printf( "checkProofOfWork :: 002 pvContextAlloc was malloced.\n" );
		#endif

		void * pvContext	= (void*)( ( (long long)pvContextAlloc + 4095 ) & -4096 );
		#ifdef _DEBUG
			printf( "checkProofOfWork :: 003 pvContext was set.\n" );
		#endif

		void * pvContextEnd	= (char*)pvContext + TRUSTNOTE_MINER_CONTEXT_SIZE;
		#ifdef _DEBUG
			printf( "checkProofOfWork :: 004 memory allocated successfully\n" );
		#endif

		//
		//	calculate ...
		//
		EhPrepare( pvContext, (void *)pcutInputHeader );
		#ifdef _DEBUG
			printf( "checkProofOfWork :: 030 EhPrepare done.\n" );
		#endif

		int32_t nSolutionCount = EhSolver( pvContext, uNonce );
		#ifdef _DEBUG
			printf( "checkProofOfWork :: 031 EhSolver returned with : %d solutions\n", nSolutionCount );
		#endif

		for ( int n = 0; n < nSolutionCount; n ++ )
		{
			//
			//	blake2b	512/256
			//	blake2s	256/128
			//
			#ifdef _DEBUG
				printf( "checkProofOfWork :: 050 will make blake2b\n" );
			#endif
			blake2b( (uint8_t *)utOutContext, (uint8_t *)pvContext + n * 1344, NULL, sizeof( utOutContext ), 1344, 0 );
			#ifdef _DEBUG
				printf( "checkProofOfWork :: 051 will convert blake2b buffer to hex string\n" );
			#endif
			bytesToHexString( utOutContext, 32, szHashHex );
			#ifdef _DEBUG
				printf( "checkProofOfWork :: 052 blake2b buffer was converted to hex string\n" );
			#endif

			#ifdef _DEBUG
				printf( "checkProofOfWork :: 100 will compare two hash hex strings.\n" );
			#endif
			if ( 0 == strncasecmp( szHashHex, pcszHashHex, 64 ) )
			{
				//
				//	hex value matched
				//
				#ifdef _DEBUG
					printf( "checkProofOfWork :: 101 will filter difficulty value.\n" );
				#endif
				int nCheck = filterDifficulty( uBits, szHashHex );

				#ifdef _DEBUG
					printf( "checkProofOfWork :: 102 @@@@@@ difficulty value filter result : %d.\n", nCheck );
				#endif
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
		#ifdef _DEBUG
			printf( "checkProofOfWork :: 150 will free pvContextAlloc.\n" );
		#endif

		free( pvContextAlloc );
		pvContextAlloc = NULL;

		#ifdef _DEBUG
			printf( "checkProofOfWork :: 150 was pvContextAlloc freed.\n" );
		#endif
	}
	catch ( char * pcszError )
	{
		#ifdef _DEBUG
			printf( "checkProofOfWork :: occurred an exception : %s.\n", pcszError );
		#endif
		nRet = -2;
	}

	return nRet;
}


/**
 *	filter difficulty
 *	check proof of work
 *
 *	@param	{uint32_t}	uBits
 *	@param	{const char *}	pcszHashHex
 *	@return	{uint32_t}
 */
EXPORT_API int filterDifficulty(
	const uint32_t uBits,
	const char * pcszHashHex )
{
	bool fNegative;
	bool fOverflow;
	arith_uint256 bnTarget;
	char szHashHexCalc[ 128 ];

	if ( NULL == pcszHashHex )
	{
		return -1000;
	}
	if ( strlen( pcszHashHex ) < 64 )
	{
		#ifdef _DEBUG
			printf( "filterDifficulty :: pcszHashHex : %s, length : %zu\n", pcszHashHex, strlen( pcszHashHex ) );
		#endif
		return -1002;
	}

	//	...
	memcpy( szHashHexCalc, pcszHashHex, 64 );
	szHashHexCalc[ 64 ]	= 0;

	//	...
	bnTarget.SetCompact( uBits, & fNegative, & fOverflow );

	//	check range
	if ( fNegative || 0 == bnTarget || fOverflow || bnTarget > UintToArith256( uint256S( TRUSTNOTE_MINER_POW_LIMIT_TARGET ) ) )
	{
		//	printf("nBits below minimum work.\n");
		return -10;
	}

	//	check proof of work matches claimed amount
	if ( UintToArith256( uint256S( szHashHexCalc ) ) > bnTarget )
	{
		//	printf("hash doesn't match nBits.\n");
		return -20;
	}

	return 0;
}


/**
 *	calculate next difficulty
 *
 *	@param	{uint32_t}	uPreviousBits
 *	@param	{uint32_t}	uTimeUsed
 *	@param	{uint32_t}	uTimeStandard
 *	@return	{uint32_t}
 */
EXPORT_API uint32_t calculateNextWorkRequired(
	const uint32_t uPreviousBits,
	const uint32_t uTimeUsed,
	const uint32_t uTimeStandard )
{
	uint32_t uRet = 0;

	//	...
	const arith_uint256 bnPowMaxUInt256	= UintToArith256( uint256S( TRUSTNOTE_MINER_POW_MAX ) );
	const arith_uint256 bnPowLimitUInt256	= UintToArith256( uint256S( TRUSTNOTE_MINER_POW_LIMIT_TARGET ) );

	//	3/4 uTimeStandard + 1/4 uTimeUsed
	int64_t n64ActualTimeSpan		= ( uTimeStandard * 3 + uTimeUsed ) / 4;
	int64_t n64MinActualTimeSpan		= ( uTimeStandard * ( 100 - 16 ) ) / 100;
	int64_t n64MaxActualTimeSpan		= ( uTimeStandard * ( 100 + 32 ) ) / 100;

	//	...
	//assert( bnPowMaxUInt256 / bnPowLimitUInt256 >= consensus.nPowAveragingWindow );

	#ifdef _DEBUG
		printf( "### uPreviousBits\t: %u\n", uPreviousBits );
		printf( "### uTimeUsed\t: %u\n", uTimeUsed );
		printf( "### uTimeStandard\t: %u\n", uTimeStandard );

		printf( "### bnPowMaxUInt256\t: %u\n", bnPowMaxUInt256.GetCompact() );
		printf( "### bnPowLimitUInt256\t: %u\n", bnPowLimitUInt256.GetCompact() );

		printf( "### n64ActualTimeSpan\t: %u\n", n64ActualTimeSpan );
		printf( "### n64MinActualTimeSpan\t: %u\n", n64MinActualTimeSpan );
		printf( "### n64MaxActualTimeSpan\t: %u\n", n64MaxActualTimeSpan );
	#endif

	if ( n64ActualTimeSpan < n64MinActualTimeSpan )
	{
		//	84% adjustment up
		#ifdef _DEBUG
			printf( "*** 84%% adjustment up, span less then min: %lld, reset as min\n", n64MinActualTimeSpan );
		#endif
		n64ActualTimeSpan = n64MinActualTimeSpan;
	}
	if ( n64ActualTimeSpan > n64MaxActualTimeSpan )
	{
		// 	132% adjustment down
		#ifdef _DEBUG
			printf( "*** 132%% adjustment down, span greater than max: %lld, reset as max\n", n64MaxActualTimeSpan );
		#endif
		n64ActualTimeSpan = n64MaxActualTimeSpan;
	}

	//
	//	retarget
	//
	//printf("bnPowLimit: %d\n", bnPowLimit.GetCompact());

	//	...
	arith_uint256 bnNewUInt256;
	bnNewUInt256.SetCompact( uPreviousBits );

	#ifdef _DEBUG
		printf( "*** before\t\t: %u\n", bnNewUInt256.GetCompact() );
	#endif

	//	...
	bnNewUInt256	/= uTimeStandard;
	bnNewUInt256	*= n64ActualTimeSpan;

	#ifdef _DEBUG
		printf( "*** after\t\t: %u [ ( %u / %u ) * %u ]\n", bnNewUInt256.GetCompact(),
			uPreviousBits, uTimeStandard, n64ActualTimeSpan );
	#endif

	//
	//	set as the limited lowest difficulty
	//
	if ( bnNewUInt256 > bnPowLimitUInt256 )
	{
		#ifdef _DEBUG
			printf( "*** bnNewUInt256(%u) > bnPowLimitUInt256(%u)\n", bnNewUInt256.GetCompact(), bnPowLimitUInt256.GetCompact() );
		#endif
		bnNewUInt256 = bnPowLimitUInt256;
	}

	//	...
	uRet = bnNewUInt256.GetCompact();
	#ifdef _DEBUG
		printf( "calculateNextWorkRequired :: uPreviousBits : %u, uTimeUsed : %u, uTimeStandard : %u, return : %u\n",
			uPreviousBits, uTimeUsed, uTimeStandard, uRet );
		printf( "\n\n\n" );
	#endif

	return uRet;
}


/**
 *	calculate next work required target in 32 bits format with deposit and round index
 *
 *	@param	{uint32_t}	uPreviousBits
 *	@param	{uint32_t}	uTimeUsed
 *	@param	{uint32_t}	uTimeStandard
 *	@param	{double}	dblDeposit		- Notes
 *	@param	{uint32_t}	uRoundIndex
 *	@return	{uint32_t}
 */
EXPORT_API uint32_t calculateNextWorkRequiredWithDeposit(
	const uint32_t uPreviousBits,
	const uint32_t uTimeUsed,
	const uint32_t uTimeStandard,
	const double   dblDeposit,
	const uint32_t uBombExplodingRoundIndex,
	const uint32_t uRoundIndex )
{
	uint32_t uRet	= 0;
	int nShift	= TRUSTNOTE_MINER_DEPOSIT_DEFAULT_SHIFT;
	arith_uint256 bnNormalUInt256;

	//
	//	calculate normal bits
	//
	bnNormalUInt256.SetCompact( calculateNextWorkRequired( uPreviousBits, uTimeUsed, uTimeStandard ) );

	//
	//	get shift by deposit
	//
	nShift		= TrustNoteDeposit::getShiftByDeposit( dblDeposit );
	if ( ! TrustNoteDeposit::isValidShift( nShift ) )
	{
		nShift	= TRUSTNOTE_MINER_DEPOSIT_DEFAULT_SHIFT;
	}

	//
	//	shift bits by deposit
	//
	if ( nShift < 0 )
	{
		//	shift right makes the result harder
		bnNormalUInt256 >>= abs( nShift );
	}
	else if ( nShift > 0 )
	{
		//	shift left makes the result easier
		for ( uint32_t i = 0; i < nShift; i ++ )
		{
			uint32_t uExponent = getExponentOfBits( bnNormalUInt256.GetCompact() );
			if ( uExponent < 32 )
			{
				bnNormalUInt256 <<= 1;
			}
		}
	}

	//
	//	shift bits by difficulty bomb
	//	Math.pow( 2, Math.floor( block.number / 100000 ) - 2 )
	//
	int nShiftBomb	= TrustNoteDifficultyBomb::getBombShiftByRoundIndex( uBombExplodingRoundIndex, uRoundIndex );
	if ( nShiftBomb < 0 )
	{
		//	shift right makes the result harder
		bnNormalUInt256 >>= abs( nShiftBomb );
	}

	//
	//	make sure the bits value is not easier than the limit
	//
	if ( _isEasierThanLimitByArithUInt256( bnNormalUInt256 ) )
	{
		uRet	= TRUSTNOTE_MINER_POW_LIMIT_BITS;
	}
	else
	{
		uRet	= bnNormalUInt256.GetCompact();
	}

	return uRet;
}


/**
 *	calculate shift by deposit
 *
 *	@param	{double}	dblDeposit
 *	@return	{uint32_t}
 */
EXPORT_API int calculateShiftByDeposit( double dblDeposit )
{
	return TrustNoteDeposit::getShiftByDeposit( dblDeposit );
}


/**
 *	calculate shift by round index
 *
 *	@param	{uint32_t}	uRoundIndex
 *	@return	{uint32_t}
 */
EXPORT_API int calculateShiftByRoundIndex( uint32_t uBombExplodingRoundIndex, uint32_t uRoundIndex )
{
	return TrustNoteDifficultyBomb::getBombShiftByRoundIndex( uBombExplodingRoundIndex, uRoundIndex );
}



/**
 *	check if the uBits is easy than limit
 *
 *	@param	{uint32_t}	uBits		e.g.: 0x1c03a809
 *	@return	{bool}
 */
EXPORT_API bool isEasierThanLimitByBits( const uint32_t uBits )
{
	arith_uint256 bnUInt256;
	bnUInt256.SetCompact( uBits );

	return _isEasierThanLimitByArithUInt256( bnUInt256 );
}
static bool _isEasierThanLimitByArithUInt256( const arith_uint256 & clsArithUInt256 )
{
	return clsArithUInt256 > UintToArith256( uint256S( TRUSTNOTE_MINER_POW_LIMIT_TARGET ) );
}



/**
 *	get work required target limit in format string 256 bits
 *
 *	@return	{int}
 *		0	success
 */
EXPORT_API int getLimitInTarget( OUT char * pszTargetHex, uint32_t uSize )
{
	if ( NULL == pszTargetHex )
	{
		#ifdef _DEBUG
			printf( "getLimitInTarget :: invalid parameter pszTargetHex.\n" );
		#endif
		return -1000;
	}
	if ( uSize < 64 )
	{
		#ifdef _DEBUG
			printf( "getLimitInTarget :: invalid parameter uSize, must be great or equal to 64.\n" );
		#endif
		return -1001;
	}

	snprintf( pszTargetHex, uSize, "%s", TRUSTNOTE_MINER_POW_LIMIT_TARGET );
	return 0;
}


/**
 *	get work required target limit in format unsigned int 32 bits
 *
 *	@return	{uint32_t}
 */
EXPORT_API uint32_t getLimitInBits()
{
	return UintToArith256( uint256S( TRUSTNOTE_MINER_POW_LIMIT_TARGET ) ).GetCompact();
}


/**
 *	convert 256 bits string target to 32 bits uint32_t bits
 *
 *	@param 	{const char *}	pcszTargetHex	"00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
 *	@return	{uint32_t}
 */
EXPORT_API uint32_t getBitsByTarget( const char * pcszTargetHex )
{
	if ( NULL == pcszTargetHex )
	{
		#ifdef _DEBUG
			printf( "getBitsByTarget :: invalid parameter pcszTargetHex, NULL.\n" );
		#endif
		return 0;
	}
	if ( strlen( pcszTargetHex ) < 64 )
	{
		#ifdef _DEBUG
			printf( "getBitsByTarget :: invalid parameter pcszTargetHex, must be 64 characters.\n" );
		#endif
		return 0;
	}

	char szTargetHex[ 65 ];
	uint256 un256Difficulty;
	arith_uint256 bn256Difficulty;

	memset( szTargetHex, 0, sizeof(szTargetHex) );
	memcpy( szTargetHex, pcszTargetHex, 64 );

	un256Difficulty	= uint256S( szTargetHex );
	bn256Difficulty	= UintToArith256( un256Difficulty );

	//	...
	return bn256Difficulty.GetCompact();
}



/**
 *	convert 32 bits uint32_t bits to 256 bits string target
 *
 *	@param 	{uint32_t}	uBits
 *	@param 	{char *}	pszTargetHex	"00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
 *	@param 	{uint32_t}	uSize
 *	@return	{int}
 */
EXPORT_API int getTargetByBits( uint32_t uBits, OUT char * pszTargetHex, uint32_t uSize )
{
	if ( NULL == pszTargetHex )
	{
		#ifdef _DEBUG
			printf( "getTargetByBits :: invalid parameter pszTargetHex, NULL.\n" );
		#endif
		return -1000;
	}
	if ( uSize < 64 )
	{
		#ifdef _DEBUG
			printf( "getTargetByBits :: invalid parameter uSize, must be great or equal to 64.\n" );
		#endif
		return -1001;
	}

	//	...
	int nRet = -1;

	//
	//	About bits
	//	@see	http://learnmeabitcoin.com/glossary/bits
	//
	//	bits			: 0x1C033AB2
	//	exponent		: 0x1C
	//	coefficient		:   0x035DB8
	//	leading zero in bytes	: 32 - exponent
	//				  32 - 28 = 4
	//
	//	that means:
	//	target			: 00000000 035DB8 FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
	//

	//
	//	the first 1 byte means
	//		the size of the target in bytes
	//
	uint32_t uExponent		= getExponentOfBits( uBits );
	if ( uExponent <= 32 )
	{
		//
		//	leading zero count in bytes
		//
		uint32_t uLeadingZeroBytes	= 32 - uExponent;

		//
		//	the initial 3 bytes of the target
		//
		uint32_t uCoefficient		= getCoefficientOfBits( uBits );

		//	...
		memset( pszTargetHex, 0, uSize );
		sprintf( pszTargetHex, "%.*x", uLeadingZeroBytes * 2, 0 );
		sprintf( pszTargetHex + uLeadingZeroBytes * 2, "%06x", uCoefficient );
		sprintf( pszTargetHex + uLeadingZeroBytes * 2 + 6, "%*c", ( uExponent - 3 ) * 2, 'f' );

		for ( uint32_t i = 0; i < uSize; i ++ )
		{
			if ( ' ' == pszTargetHex[ i ] )
			{
				pszTargetHex[ i ] = 'f';
			}
		}

		pszTargetHex[ 64 ] = 0;

		//
		//	successfully
		//
		nRet = 0;
	}
	else
	{
		nRet = -2000;
		#ifdef _DEBUG
			printf( "getTargetByBits :: uExponent must less then or equal to 32.\n" );
		#endif
	}

	return nRet;
}


/**
 *	get exponent of a given bits
 *
 *	@param	{uint32_t}	uBits
 *	@return	{uint32_t}
 */
EXPORT_API uint32_t getExponentOfBits( uint32_t uBits )
{
	return ( uBits >> 24 );
}


/**
 *	get Coefficient of a given bits
 *
 *	@param	{uint32_t}	uBits
 *	@return	{uint32_t}
 */
EXPORT_API uint32_t getCoefficientOfBits( uint32_t uBits )
{
	return ( ( uBits << 8 ) >> 8 );
}