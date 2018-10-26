//
// Created by Liu QiXing on 2018/10/26.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <algorithm>
#include <cmath>

#include "trustnote-deposit.h"


void test_printDepositTable();
void test_getShiftByDeposit();




int main()
{
	test_printDepositTable();

	printf( "\n\n\n" );
	test_getShiftByDeposit();

	return 0;
}


void test_printDepositTable()
{
	STPOWDEPOSIT * parrDepositTable;
	int i;

	parrDepositTable	= TrustNoteDeposit::getDepositTable();
	if ( parrDepositTable )
	{
		printf( "%*s%*s%*s\n", 8, "shift", 18, "times", 16, "deposit" );
		printf( "------------------------------------------------------------\n" );

		for ( i = 0; i < TRUSTNOTE_DEPOSIT_TABLE_LENGTH; i ++ )
		{
			printf( "{ %*d,", 8, parrDepositTable[ i ].nShift );
			printf( "%*f,", 18, parrDepositTable[ i ].dblTimes );
			printf( "%*f },", 16, parrDepositTable[ i ].dblDeposit );
			printf( "\n" );
		}
	}
}

void test_getShiftByDeposit()
{
	double arrDblData[] =
	{
		-10000,
		-100.0,
		0,
		1.0,
		50.0,
		100.0,
		10000.0,
		80000.0,
		90000.0,
		100000.0
	};
	uint32_t nDblDataLength = sizeof( arrDblData ) / sizeof( arrDblData[ 0 ] );
	int i;
	double dblDeposit;
	int nShift;

	for ( i = 0; i < nDblDataLength; i ++ )
	{
		dblDeposit	= arrDblData[ i ];
		nShift		= TrustNoteDeposit::getShiftByDeposit( dblDeposit );
		printf
		(
			"test_getShiftByDeposit :: deposit : %*f. shift : %*d\n",
			16,
			dblDeposit,
			4,
			nShift
		);
	}

}