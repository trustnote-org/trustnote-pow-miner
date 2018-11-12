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

#include "trustnote-miner-deposit.h"


/**
 *	structure
 */
typedef struct tagPowDepositTestData
{
	tagPowDepositTestData()
	{
		memset( this, 0, sizeof( tagPowDepositTestData ) );
	}
	tagPowDepositTestData( bool bExpect_, int nShouldShift_, double dblDeposit_ )
	{
		bExpect		= bExpect_;
		nShouldShift	= nShouldShift_;
		dblDeposit	= dblDeposit_;
	}

	bool   bExpect;
	int    nShouldShift;
	double dblDeposit;

}STPOWDEPOSITTESTDATA, *LPSTPOWDEPOSITTESTDATA;


void test_getShiftByDeposit();



int main()
{
	test_getShiftByDeposit();

	return 0;
}

void test_getShiftByDeposit()
{
	static STPOWDEPOSITTESTDATA arrDblDataTestTable[] =
	{
		{ true,	0,	       -100.0 },
		{ true,	0,	          0.0 },
		{ true,	0,	          1.0 },
		{ true,	0,	        800.0 },
		{ true,	0,	       1000.0 },
		{ true,	1,	      10001.0 },
		{ true,	2,	      10751.0 },
		{ true,	3,	      20002.0 },
		{ true,	4,	      40003.0 },
		{ true,	5,	      53004.0 },
		{ true,	6,	      90005.0 },
		{ true,	7,	     119006.0 },
		{ true,	8,	     176006.0 },
		{ true,	9,	     257006.0 },
		{ true,	10,	     375006.0 },
		{ true,	11,	     544006.0 },
		{ true,	12,	    1000007.0 },
		{ true,	13,	    1139000.0 },
		{ true,	14,	    1659000.0 },
		{ true,	15,	    2389000.0 },
		{ true,	16,	    3424000.0 },
		{ true,	17,	    4936000.0 },
		{ true,	18,	   10000008.0 },
		{ true,	19,	   10260008.0 },
		{ true,	20,	   14776000.0 },
		{ true,	25,	  100000009.0 },
		{ true,	31,	 1000000010.0 },
		{ true,	32,	 2000000011.0 },
		{ true,	32,	 5000000012.0 },
		{ true,	32,	50000000013.0 },
	};
	uint32_t nDblDataLength = sizeof( arrDblDataTestTable ) / sizeof( arrDblDataTestTable[ 0 ] );
	int i;
	double dblDeposit;
	bool bExpect;
	int  nCalcShift;
	int  nShouldShift;

	for ( i = 0; i < nDblDataLength; i ++ )
	{
		bExpect		= arrDblDataTestTable[ i ].bExpect;
		nShouldShift	= arrDblDataTestTable[ i ].nShouldShift;
		dblDeposit	= arrDblDataTestTable[ i ].dblDeposit * 1000.0 * 1000.0;
		nCalcShift	= TrustNoteDeposit::getShiftByDeposit( dblDeposit );

		printf
		(
			"deposit : %*f. shift : %*d, %*d\t--> %s\n",
			26,
			dblDeposit,
			4,
			nCalcShift,
			4,
			nShouldShift,
			( bExpect == ( nCalcShift == nShouldShift ) ) ? "OK" : "*FAILED"
		);
	}

}