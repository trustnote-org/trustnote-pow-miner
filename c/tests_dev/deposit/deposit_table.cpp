//
// Created by Liu QiXing on 2018/10/25.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <algorithm>
#include <cmath>

#include "trustnote-miner-deposit.h"




int main()
{
	int i;
	int nLength;
	STPOWDEPOSIT * parrDepositList;

	nLength	= TRUSTNOTE_MINER_DEPOSIT_TABLE_LENGTH;
	if ( 0 == TrustNoteDeposit::initDepositTable() )
	{
		parrDepositList	= TrustNoteDeposit::getDepositTable();
		printf( "%*s%*s%*s\n", 10, "shift >>", 30, "times", 29, "deposit" );
		printf( "----------------------------------------------------------------------\n" );

		for ( i = 0; i < nLength; i ++ )
		{
			printf( "{ %*d,", 8, parrDepositList[ i ].nShift );
			printf( "%*f,", 29, parrDepositList[ i ].dblTimes );
			printf( "%*f },", 28, parrDepositList[ i ].dblDeposit );
			printf( "\n" );
		}
	}
	else
	{
		printf( "failed to init deposit table." );
	}
}