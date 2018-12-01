//
// Created by Liu QiXing on 2018/11/26.
//
//
// Created by Liu QiXing on 2018/10/29.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <algorithm>
#include <cmath>

#include "trustnote-difficulty-bomb.h"
#include "test_difficulty_bomb.h"



void test_difficulty_bomb();
void test_difficulty_bomb_single();




int main()
{
	test_difficulty_bomb();

	test_difficulty_bomb_single();

	return 0;
}


void test_difficulty_bomb()
{
	uint32_t uExplodingRoundIndex	= 50;
	int arrTestData[][ 2 ] =
		{
			{ 0,		300 },
//			{ 100000,	200000 },
//			{ 200000,	300000 },
//			{ 300000,	400000 },
//			{ 400000,	500000 },
//			{ 500000,	600000 },
//			{ 600000,	700000 },
//			{ 700000,	800000 },
//			{ 800000,	900000 },
		};
	int nLength	= sizeof( arrTestData ) / sizeof( arrTestData[ 0 ] );

	printf( "\n" );
	printf( "%*s", 8, "Start" );
	printf( "%*s", 8, "End" );
	printf( "%*s", 16, "Round" );
	printf( "%*s", 8, "Shift" );
	printf( "\n" );
	printf( "------------------------------------------------------------\n" );

	for ( int i = 0; i < nLength; i ++ )
	{
		int nRoundIndexStart	= arrTestData[ i ][ 0 ];
		int nRoundIndexEnd	= arrTestData[ i ][ 1 ];

		for ( int j = nRoundIndexStart; j < nRoundIndexEnd; j += 1 )
		{
			int nBombShift	= TrustNoteDifficultyBomb::getBombShiftByRoundIndex( uExplodingRoundIndex, j );

			printf( "%*d", 8, nRoundIndexStart );
			printf( "%*d", 8, nRoundIndexEnd );
			printf( "%*d", 16, j );
			printf( "%*d", 8, nBombShift );
			printf( "\n" );
		}

		printf( "\n" );
	}

	printf( "Done!\n\n" );
}


void test_difficulty_bomb_single()
{
	uint32_t uExplodingRoundIndex	= 10000;
	uint32_t uRoundIndex		= 170000;
	int nBombShift			= TrustNoteDifficultyBomb::getBombShiftByRoundIndex( uExplodingRoundIndex, uRoundIndex );

	printf( "* uExplodingRoundIndex\t: %u\n", uExplodingRoundIndex );
	printf( "* uRoundIndex\t\t: %u\n", uRoundIndex );
	printf( "* nBombShift\t\t: %d\n", nBombShift );
	printf( "\n" );

	printf( "Done!\n\n" );
}
