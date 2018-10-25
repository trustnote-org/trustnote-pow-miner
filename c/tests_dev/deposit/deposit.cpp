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


typedef struct tagPowDeposit
{
    tagPowDeposit()
    {
	init();
    }
    void init()
    {
	    memset( this, 0, sizeof( tagPowDeposit ) );
    }

    int	   nShift;
    double dblTimes;
    double dblDeposit;

}STPOWDEPOSIT, *LPSTPOWDEPOSIT;


int initPowDepositTable( STPOWDEPOSIT * pstPowDepositList, uint32_t nLength );




int main()
{
	int i;
	int nLength;
	STPOWDEPOSIT arrDepositList[ 65 ];

	nLength	= sizeof( arrDepositList ) / sizeof( arrDepositList[ 0 ] );
	initPowDepositTable( arrDepositList, nLength );

	printf( "%*s%*s%*s\n", 8, "shift", 18, "times", 16, "deposit" );
	printf( "------------------------------------------------------------\n" );

	for ( i = 0; i < nLength; i ++ )
	{
		printf( "%*d", 8, arrDepositList[ i ].nShift );
		printf( "%*f", 18, arrDepositList[ i ].dblTimes );
		printf( "%*f", 16, arrDepositList[ i ].dblDeposit );
		printf( "\n" );
	}
}


int initPowDepositTable( STPOWDEPOSIT * pstPowDepositList, uint32_t nLength )
{
	int i;
	int nShift;

	for ( i = 0; i < nLength; i ++ )
	{
		nShift	= i - 32;

		pstPowDepositList[ i ].nShift		= nShift;
		pstPowDepositList[ i ].dblTimes		= pow( 2, nShift );
		pstPowDepositList[ i ].dblDeposit	= 50 * pow( pstPowDepositList[ i ].dblTimes, (double)( 1.0 / 3.0 ) );
	}

	return 0;
}