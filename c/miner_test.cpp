//
// Created by Liu QiXing on 2018/10/25.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <algorithm>

#include "trustnote-miner-deposit.h"
#include "trustnote-difficulty-bomb.h"
#include "miner.h"
#include "miner_test.h"



/**
 *	main
 *
 *	@return	{int}
 */
int main( void )
{
	miner_test_print_header( "miner_test_checkProofOfWork" );
	miner_test_checkProofOfWork();
	miner_test_print_footer();

	miner_test_print_header( "miner_test_getLimitInBits" );
	miner_test_getLimitInBits();
	miner_test_print_footer();

	miner_test_print_header( "miner_test_getLimitInTarget" );
	miner_test_getLimitInTarget();
	miner_test_print_footer();

	miner_test_print_header( "miner_test_calculateNextWorkRequired" );
	miner_test_calculateNextWorkRequired();
	miner_test_print_footer();

	miner_test_print_header( "miner_test_calculateNextWorkRequiredWithDeposit" );
	miner_test_calculateNextWorkRequiredWithDeposit();
	miner_test_print_footer();

	miner_test_print_header( "miner_test_startMining" );
	miner_test_startMining();
	miner_test_print_footer();
}


void miner_test_print_header( const char * pcszHeader )
{
	printf( "------------------------------------------------------------\n" );
	printf( "- %s", pcszHeader ? pcszHeader : "NULL" );
	printf( "\n------------------------------------------------------------\n" );
}

void miner_test_print_footer( const char * pcszFooter /* = NULL */ )
{
	if ( pcszFooter )
	{
		printf( "%s", pcszFooter );
	}

	printf( "\n>>> Done!\n\n\n\n" );
}





void miner_test_checkProofOfWork()
{
	uint8_t utInputHeader[ 140 ]	= { 229,130,52,193,179,170,97,242,245,204,126,226,139,52,226,73,215,54,190,179,35,159,207,112,11,194,17,0,146,112,29,22,94,126,70,238,46,127,228,102,205,247,71,67,178,9,152,34,122,245,79,202,84,180,188,222,140,216,50,120,102,215,17,48,252,233,209,133,206,33,66,137,14,93,141,120,186,6,9,38,98,154,121,93,230,229,244,95,14,254,249,184,151,240,132,74,14,2,236,58,130,144,171,51,31,170,90,247,28,222,203,81,136,214,60,140,54,107,213,236,71,121,12,153,166,213,6,47,43,132,202,221,98,255,86,156,194,127,204,240 };
	uint32_t uBits			= 528482303;
	uint32_t uNonce			= 65;
	char szHexHash[ 64 ];

	memset( szHexHash, 0, sizeof( szHexHash ) );
	memcpy( szHexHash, "00198bb0606e5a8b5d47577bc96de488116af886815f4dccc5ad1ebd78d1b14e", 64 );

	int nCheck1	= checkProofOfWork( NULL, uBits, uNonce, szHexHash );
	printf( "miner_test_checkProofOfWork 1 : %d \n", nCheck1 );

	int nCheck2	= checkProofOfWork( utInputHeader, uBits, uNonce, NULL );
	printf( "miner_test_checkProofOfWork 2 : %d \n", nCheck2 );

	int nCheck3	= checkProofOfWork( utInputHeader, uBits, uNonce, szHexHash );
	printf( "miner_test_checkProofOfWork 3 : %d \n", nCheck3 );
}

void miner_test_getLimitInBits()
{
	uint32_t uBits	= getLimitInBits();
	char szTarget[ 66 ];
	int nCall;

	memset( szTarget, 0, sizeof( szTarget ) );
	nCall = getTargetByBits( uBits, szTarget, sizeof( szTarget ) );

	printf( "miner_test_getLimitInBits : %08x, %d \n", uBits, uBits );
	printf( "miner_test_getLimitInBits : %d, %s \n", nCall, szTarget );
}

void miner_test_getLimitInTarget()
{
	char szTarget[ 66 ];
	uint32_t uBits;
	int nCall;

	memset( szTarget, 0, sizeof( szTarget ) );

	nCall	= getLimitInTarget( szTarget, sizeof( szTarget ) );
	uBits	= getBitsByTarget( szTarget );
	printf( "miner_test_getLimitInTarget : %d, %s \n", nCall, szTarget );
	printf( "miner_test_getLimitInTarget : %x, %d \n", uBits, uBits );
}

void miner_test_calculateNextWorkRequired()
{
	#define MINER_TEST_CALCULATENEXTWORKREQUIRED_BITS	528482303

	uint32_t arrDataList[][ 3 ] =
	{
		{ MINER_TEST_CALCULATENEXTWORKREQUIRED_BITS, 3194,  2400 },
		{ MINER_TEST_CALCULATENEXTWORKREQUIRED_BITS, 2294,  2400 },
		{ MINER_TEST_CALCULATENEXTWORKREQUIRED_BITS, 1194,  2400 },
		{ MINER_TEST_CALCULATENEXTWORKREQUIRED_BITS, 30000, 15000 },
		{ MINER_TEST_CALCULATENEXTWORKREQUIRED_BITS, 10000, 15000 },
		{ MINER_TEST_CALCULATENEXTWORKREQUIRED_BITS, 40000, 15000 },
		{ MINER_TEST_CALCULATENEXTWORKREQUIRED_BITS, 50000, 15000 },
		{ MINER_TEST_CALCULATENEXTWORKREQUIRED_BITS, 60000, 15000 },
		{ MINER_TEST_CALCULATENEXTWORKREQUIRED_BITS, 70000, 15000 },
		{ MINER_TEST_CALCULATENEXTWORKREQUIRED_BITS, 80000, 15000 },
		{ MINER_TEST_CALCULATENEXTWORKREQUIRED_BITS, 90000, 15000 },
	};
	char szNewTarget[ 66 ];

	printf( "miner_test_calculateNextWorkRequired : \n" );
	for ( uint32_t i = 0; i < sizeof( arrDataList ) / sizeof( arrDataList[ 0 ] ); i ++ )
	{
		memset( szNewTarget, 0, sizeof( szNewTarget ) );
		uint32_t uNewBits	= calculateNextWorkRequired( arrDataList[ i ][ 0 ], arrDataList[ i ][ 1 ], arrDataList[ i ][ 2 ] );
		getTargetByBits( uNewBits, szNewTarget, sizeof( szNewTarget ) );

		printf( "\tuPreviousBits\t: %08x, %d\n", arrDataList[ i ][ 0 ], arrDataList[ i ][ 0 ] );
		printf( "\tuTimeUsed\t: %08x, %d\n", arrDataList[ i ][ 1 ], arrDataList[ i ][ 1 ] );
		printf( "\tuTimeStandard\t: %08x, %d\n", arrDataList[ i ][ 2 ], arrDataList[ i ][ 2 ] );
		printf( "\tuNewBits\t: %08x, %d\n", uNewBits, uNewBits );
		printf( "\tszNewTarget\t: %s\n", szNewTarget );
		printf( "\n" );
	}
}

void miner_test_calculateNextWorkRequiredWithDeposit()
{
	//	0x1b07ffff	= 0000000000000000000000000000000007ffffffffffffffffffffffffffffff
	#define MINER_TEST_CALCULATENEXTWORKREQUIRED_WITHDEPOSIT_BITS	0x1007ffff

	uint32_t arrDataList[][ 3 ] =
		{
			{ MINER_TEST_CALCULATENEXTWORKREQUIRED_WITHDEPOSIT_BITS, 3194,  2400 },
			{ MINER_TEST_CALCULATENEXTWORKREQUIRED_WITHDEPOSIT_BITS, 2294,  2400 },
			{ MINER_TEST_CALCULATENEXTWORKREQUIRED_WITHDEPOSIT_BITS, 1194,  2400 },
			{ MINER_TEST_CALCULATENEXTWORKREQUIRED_WITHDEPOSIT_BITS, 30000, 15000 },
			{ MINER_TEST_CALCULATENEXTWORKREQUIRED_WITHDEPOSIT_BITS, 10000, 15000 },
			{ MINER_TEST_CALCULATENEXTWORKREQUIRED_WITHDEPOSIT_BITS, 40000, 15000 },
			{ MINER_TEST_CALCULATENEXTWORKREQUIRED_WITHDEPOSIT_BITS, 50000, 15000 },
			{ MINER_TEST_CALCULATENEXTWORKREQUIRED_WITHDEPOSIT_BITS, 60000, 15000 },
			{ MINER_TEST_CALCULATENEXTWORKREQUIRED_WITHDEPOSIT_BITS, 70000, 15000 },
			{ MINER_TEST_CALCULATENEXTWORKREQUIRED_WITHDEPOSIT_BITS, 80000, 15000 },
			{ MINER_TEST_CALCULATENEXTWORKREQUIRED_WITHDEPOSIT_BITS, 90000, 15000 },
		};
	double arrDblData[] =
		{
			-100.0,
			0,
			1.0		* 10000.0 * 1000.0 * 1000.0,
			50.0		* 10000.0 * 1000.0 * 1000.0,
			100.0		* 10000.0 * 1000.0 * 1000.0,
			10000.0		* 10000.0 * 1000.0 * 1000.0,
			80000.0		* 10000.0 * 1000.0 * 1000.0,
			90000.0		* 10000.0 * 1000.0 * 1000.0,
			100000.0	* 10000.0 * 1000.0 * 1000.0,
			200000.0	* 10000.0 * 1000.0 * 1000.0,
			500000.0	* 10000.0 * 1000.0 * 1000.0,
			1000000.0	* 10000.0 * 1000.0 * 1000.0,
			2000000.0	* 10000.0 * 1000.0 * 1000.0,
			10000000.0	* 10000.0 * 1000.0 * 1000.0,
			100000000.0	* 10000.0 * 1000.0 * 1000.0,
			500000000.0	* 10000.0 * 1000.0 * 1000.0,
			900000000.0	* 10000.0 * 1000.0 * 1000.0,
		};
	int arrRoundIndex[] =
		{
			1000,
			100000,
			300000,
			500000,
			1000000,
			2000000
		};

	for ( uint32_t i = 0; i < sizeof( arrDataList ) / sizeof( arrDataList[ 0 ] ); i ++ )
	{
		for ( uint32_t x = 0; x < sizeof( arrDblData ) / sizeof( arrDblData[ 0 ] ); x ++ )
		{
			double dblDeposit	= arrDblData[ x ];

			for ( uint32_t r = 0; r < sizeof( arrRoundIndex ) / sizeof( arrRoundIndex[ 0 ] ); r ++ )
			{
				uint32_t uRoundIndex	= arrRoundIndex[ r ];

				//
				//	...
				//
				uint32_t uNewBits;
				char szPrevTarget[ 66 ];
				char szNewTarget[ 66 ];

				memset( szPrevTarget, 0, sizeof( szPrevTarget ) );
				getTargetByBits( arrDataList[ i ][ 0 ], szPrevTarget, sizeof( szPrevTarget ) );

				memset( szNewTarget, 0, sizeof( szNewTarget ) );
				uNewBits = calculateNextWorkRequired( arrDataList[ i ][ 0 ], arrDataList[ i ][ 1 ], arrDataList[ i ][ 2 ] );
				getTargetByBits( uNewBits, szNewTarget, sizeof( szNewTarget ) );

				//	...
				int nShiftWithDeposit;
				uint32_t uNewBitsWithDeposit;
				char szNewTargetWithDeposit[ 66 ];

				nShiftWithDeposit	= TrustNoteDeposit::getShiftByDeposit( dblDeposit );
				memset( szNewTargetWithDeposit, 0, sizeof( szNewTargetWithDeposit ) );
				uNewBitsWithDeposit = calculateNextWorkRequiredWithDeposit
					(
						arrDataList[ i ][ 0 ],
						arrDataList[ i ][ 1 ],
						arrDataList[ i ][ 2 ],
						dblDeposit,
						0
					);
				getTargetByBits( uNewBitsWithDeposit, szNewTargetWithDeposit, sizeof( szNewTargetWithDeposit ) );

				//	...
				//	...
				int nShiftBomb;
				uint32_t uNewBombBits;
				char szNewBombTarget[ 66 ];

				nShiftBomb	= TrustNoteDifficultyBomb::getBombShiftByRoundIndex( uRoundIndex );
				memset( szNewBombTarget, 0, sizeof( szNewBombTarget ) );
				uNewBombBits	= calculateNextWorkRequiredWithDeposit
					(
						arrDataList[ i ][ 0 ],
						arrDataList[ i ][ 1 ],
						arrDataList[ i ][ 2 ],
						dblDeposit,
						uRoundIndex
					);
				getTargetByBits( uNewBombBits, szNewBombTarget, sizeof( szNewBombTarget ) );


				//	...
				printf( "%*s: %08x\n", 16, "PrevBits", arrDataList[ i ][ 0 ] );
				printf( "%*s: %s\n", 16, "PrevTarget", szPrevTarget );
				printf( "%*s: %d\n", 16, "TimeUsed", arrDataList[ i ][ 1 ] );
				printf( "%*s: %d\n", 16, "TimeStd", arrDataList[ i ][ 2 ] );

				printf( "%*s: %08x\n", 16, "newStdBits", uNewBits );
				printf( "%*s: %s\n", 16, "newStdTarget", szNewTarget );

				printf( "%*s: %f WMN ||| %f\n", 16, "Deposit", ( dblDeposit / ( 10000.0 * 1000.0 * 1000.0 ) ), dblDeposit );
				printf( "%*s: %d\n", 16, "DepositShift", nShiftWithDeposit );
				printf( "%*s: %08x\n", 16, "newDepositBits", uNewBitsWithDeposit );
				printf( "%*s: %s\n", 16, "newDepositTarget", szNewTargetWithDeposit );

				printf( "%*s: %d\n", 16, "uRoundIndex", uRoundIndex );
				printf( "%*s: %d\n", 16, "BombShift", nShiftBomb );
				printf( "%*s: %08x\n", 16, "uNewBombBits", uNewBombBits );
				printf( "%*s: %s\n", 16, "szNewBombTarget", szNewBombTarget );
				printf( "\n" );
			}
		}

		printf( "------------------------------------------------------------\n\n" );
	}
}

void miner_test_startMining()
{
	uint8_t utInputHeader[ 140 ];

	uint32_t uBits			= getLimitInBits();
	char szTarget[ 66 ];
	uint32_t uNonceStart		= 0;
	uint32_t uCalcTimes		= 30000;
	uint32_t uNonce;
	char szHexHash[ 64 ];
	int nCheckPoW;


	getTargetByBits( uBits, szTarget, sizeof( szTarget ) );
	printf( "miner_test_startMining : will start mining using CPU by 1 core\n" );
	printf( "miner_test_startMining : bits %08x, %u\n", uBits, uBits );
	printf( "miner_test_startMining : target %s\n", szTarget );

	//	...
	memset( utInputHeader, 0, sizeof( utInputHeader ) );
//	for ( int i = 0; i < 140; i ++ )
//	{
//		utInputHeader[ i ] = i;
//	}
	int nCallStartMining = startMining( utInputHeader, uBits, uNonceStart, uCalcTimes, &uNonce, szHexHash, sizeof( szHexHash ) );
	printf
	(
		"miner_test_startMining : %d, nonce : %u\t, hex : %.*s\n",
		nCallStartMining,
		uNonce,
		64,
		szHexHash
	);

	//	...
	nCheckPoW = checkProofOfWork( utInputHeader, uBits, uNonce, szHexHash );
	printf( "miner_test_startMining : checkProofOfWork = %d : %s\n\n\n\n",
			nCheckPoW,
			0 == nCheckPoW ? "Winer Winner Chicken Dinner!" : "Not Okay!" );
}