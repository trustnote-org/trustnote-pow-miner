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


int getBombShiftByRoundIndex( int nRoundIndex );
void test_ethDifficultyBomb();



int main()
{
	test_ethDifficultyBomb();
}


int getBombShiftByRoundIndex( int nRoundIndex )
{
	//	Math.pow( 2, Math.floor( block.number / 100000 ) - 2 )
	return -1 * std::min( (int)( pow( 2, floor( nRoundIndex / 100000 ) - 2.0 ) ), 32 );
}


void test_ethDifficultyBomb()
{
	int arrTestData[][ 2 ] =
	{
		{ 0,		100000 },
		{ 100000,	200000 },
		{ 200000,	300000 },
		{ 300000,	400000 },
		{ 400000,	500000 },
		{ 500000,	600000 },
		{ 600000,	700000 },
		{ 700000,	800000 },
		{ 800000,	900000 },
	};
	int nLength	= sizeof( arrTestData ) / sizeof( arrTestData[ 0 ] );

	printf( "\n" );
	printf( "%*s", 8, "Start" );
	printf( "%*s", 8, "End" );
	printf( "%*s", 8, "Index" );
	printf( "%*s", 8, "Shift" );
	printf( "\n" );
	printf( "------------------------------------------------------------\n" );
	printf( "\n" );

	for ( int i; i < nLength; i ++ )
	{
		int nRoundIndexStart	= arrTestData[ i ][ 0 ];
		int nRoundIndexEnd	= arrTestData[ i ][ 1 ];

		for ( int j = nRoundIndexStart; j < nRoundIndexEnd; j += 5000 )
		{
			int nBombShift	= getBombShiftByRoundIndex( j );

			printf( "%*d", 8, nRoundIndexStart );
			printf( "%*d", 8, nRoundIndexEnd );
			printf( "%*d", 8, j );
			printf( "%*d", 8, nBombShift );
			printf( "\n" );
		}

		printf( "\n" );
	}

	printf( "Done!\n" );

	double a = pow( 2.0, -4.0 );
	printf( "pow(2,-32) = %f, %d\n\n", a, (int)a );
}



