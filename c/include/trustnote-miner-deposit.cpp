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
 *	private member variables
 */
static STPOWDEPOSIT _arrPowDepositTable[ TRUSTNOTE_MINER_DEPOSIT_TABLE_LENGTH ] =
{
	//
	//  nShift            dblTimes		         dblDeposit
	//
	{        0,           1.000000,                    0.000000 },
	{        1,           1.500000,           4403967511.883271 },
	{        2,           2.250000,          10747428008.338873 },
	{        3,           3.375000,          19884527898.725021 },
	{        4,           5.062500,          33045576896.120438 },
	{        5,           7.593750,          52002709114.199196 },
	{        6,          11.390625,          79308500772.967407 },
	{        7,          17.085938,         118639674366.882446 },
	{        8,          25.628906,         175292169031.981781 },
	{        9,          38.443359,         256894238294.304962 },
	{       10,          57.665039,         374433593750.000000 },
	{       11,          86.497559,         543736899485.153198 },
	{       12,         129.746338,         787600831031.511841 },
	{       13,         194.619507,        1138861645762.899658 },
	{       14,         291.929260,        1644816582121.755371 },
	{       15,         437.893890,        2373592428700.748047 },
	{       16,         656.840836,        3423318790457.651855 },
	{       17,         985.261253,        4935341231569.038086 },
	{       18,        1477.891880,        7113253443469.723633 },
	{       19,        2216.837820,       10250311117864.853516 },
	{       20,        3325.256730,       14768918800354.005859 },
	{       21,        4987.885095,       21277506626106.000000 },
	{       22,        7481.827643,       30652455385143.066406 },
	{       23,       11222.741464,       44156101120217.101562 },
	{       24,       16834.112196,       63606708566215.851562 },
	{       25,       25251.168294,       91623300340071.921875 },
	{       26,       37876.752441,      131978308110503.828125 },
	{       27,       56815.128662,      190105530197213.625000 },
	{       28,       85222.692992,      273831792046512.812500 },
	{       29,      127834.039489,      394430827603386.562500 },
	{       30,      191751.059233,      568141286615953.000000 },
	{       31,      287626.588849,      818353267425086.875000 },
	{       32,      431439.883274,     1178757791690959.250000 },
};





/**
 *	initialize deposit table
 *
 *	@return	{int}
 */
int TrustNoteDeposit::initDepositTable()
{
	int i;
	int nShift;

	for ( i = 0; i < TRUSTNOTE_MINER_DEPOSIT_TABLE_LENGTH; i ++ )
	{
		nShift	= i;

		_arrPowDepositTable[ i ].nShift		= nShift;
		_arrPowDepositTable[ i ].dblTimes	= pow( 1.5, (double)nShift );
		_arrPowDepositTable[ i ].dblDeposit	= 10000.0 * 1000.0 * 1000.0 * ( pow( _arrPowDepositTable[ i ].dblTimes, 0.9 ) - 1 );
	}

	//	...
	return 0;
}


/**
 *	get deposit table
 *
 *	@return	{STPOWDEPOSIT *}
 */
STPOWDEPOSIT * TrustNoteDeposit::getDepositTable()
{
	return _arrPowDepositTable;
}


/**
 *	get shift by deposit
 *
 *	@param	{double}	dblDeposit	in Notes
 *	@return	{int}
 */
int TrustNoteDeposit::getShiftByDeposit( double dblDeposit )
{
	int nRet;
	int i;

	//	...
	nRet = TRUSTNOTE_MINER_DEPOSIT_DEFAULT_SHIFT;

	for ( i = TRUSTNOTE_MINER_DEPOSIT_TABLE_LENGTH - 1; i >= 0; i -- )
	{
		if ( dblDeposit >= _arrPowDepositTable[ i ].dblDeposit )
		{
			nRet = _arrPowDepositTable[ i ].nShift;
			break;
		}
	}

	return nRet;
}


/**
 *	check if a shift value is valid
 *
 *	@param	{int}	nShift
 *	@return	{boolean}
 */
bool TrustNoteDeposit::isValidShift( int nShift )
{
	return ( nShift >= TRUSTNOTE_MINER_DEPOSIT_MIN_SHIFT && nShift <= TRUSTNOTE_MINER_DEPOSIT_MAX_SHIFT );
}
