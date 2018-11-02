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


void test_getShiftByDeposit();




int main()
{
	test_getShiftByDeposit();

	return 0;
}

void test_getShiftByDeposit()
{
	double arrDblData[] =
	{
		-100.0,
		0,
		1.0,
		800.0,
		1000.0,
		10000.0,
		20000.0,
		40000.0,
		50000.0,
		90000.0,
		100000.0,
		1000000.0,
		10000000.0,
		100000000.0,
		1000000000.0,
		2000000000.0,
		5000000000.0,
		50000000000.0,
	};
	uint32_t nDblDataLength = sizeof( arrDblData ) / sizeof( arrDblData[ 0 ] );
	int i;
	double dblDeposit;
	int nShift;

	for ( i = 0; i < nDblDataLength; i ++ )
	{
		dblDeposit	= arrDblData[ i ] * 1000.0 * 1000.0;
		nShift		= TrustNoteDeposit::getShiftByDeposit( dblDeposit );
		printf
		(
			"test_getShiftByDeposit :: deposit : %*f. shift : %*d\n",
			26,
			dblDeposit,
			4,
			nShift
		);
	}

}