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
	{      -32,          0.000000,        0.030760 },
	{      -31,          0.000000,        0.038755 },
	{      -30,          0.000000,        0.048828 },
	{      -29,          0.000000,        0.061520 },
	{      -28,          0.000000,        0.077510 },
	{      -27,          0.000000,        0.097656 },
	{      -26,          0.000000,        0.123039 },
	{      -25,          0.000000,        0.155020 },
	{      -24,          0.000000,        0.195313 },
	{      -23,          0.000000,        0.246078 },
	{      -22,          0.000000,        0.310039 },
	{      -21,          0.000000,        0.390625 },
	{      -20,          0.000001,        0.492157 },
	{      -19,          0.000002,        0.620079 },
	{      -18,          0.000004,        0.781250 },
	{      -17,          0.000008,        0.984313 },
	{      -16,          0.000015,        1.240157 },
	{      -15,          0.000031,        1.562500 },
	{      -14,          0.000061,        1.968627 },
	{      -13,          0.000122,        2.480314 },
	{      -12,          0.000244,        3.125000 },
	{      -11,          0.000488,        3.937253 },
	{      -10,          0.000977,        4.960628 },
	{       -9,          0.001953,        6.250000 },
	{       -8,          0.003906,        7.874507 },
	{       -7,          0.007812,        9.921257 },
	{       -6,          0.015625,       12.500000 },
	{       -5,          0.031250,       15.749013 },
	{       -4,          0.062500,       19.842513 },
	{       -3,          0.125000,       25.000000 },
	{       -2,          0.250000,       31.498026 },
	{       -1,          0.500000,       39.685026 },
	{        0,          1.000000,       50.000000 },
	{        1,          2.000000,       62.996052 },
	{        2,          4.000000,       79.370053 },
	{        3,          8.000000,      100.000000 },
	{        4,         16.000000,      125.992105 },
	{        5,         32.000000,      158.740105 },
	{        6,         64.000000,      200.000000 },
	{        7,        128.000000,      251.984210 },
	{        8,        256.000000,      317.480210 },
	{        9,        512.000000,      400.000000 },
	{       10,       1024.000000,      503.968420 },
	{       11,       2048.000000,      634.960421 },
	{       12,       4096.000000,      800.000000 },
	{       13,       8192.000000,     1007.936840 },
	{       14,      16384.000000,     1269.920842 },
	{       15,      32768.000000,     1600.000000 },
	{       16,      65536.000000,     2015.873680 },
	{       17,     131072.000000,     2539.841683 },
	{       18,     262144.000000,     3200.000000 },
	{       19,     524288.000000,     4031.747360 },
	{       20,    1048576.000000,     5079.683366 },
	{       21,    2097152.000000,     6400.000000 },
	{       22,    4194304.000000,     8063.494719 },
	{       23,    8388608.000000,    10159.366733 },
	{       24,   16777216.000000,    12800.000000 },
	{       25,   33554432.000000,    16126.989439 },
	{       26,   67108864.000000,    20318.733465 },
	{       27,  134217728.000000,    25600.000000 },
	{       28,  268435456.000000,    32253.978877 },
	{       29,  536870912.000000,    40637.466930 },
	{       30, 1073741824.000000,    51200.000000 },
	{       31, 2147483648.000000,    64507.957755 },
	{       32, 4294967296.000000,    81274.933861 }
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
		nShift	= i - 32;

		_arrPowDepositTable[ i ].nShift		= nShift;
		_arrPowDepositTable[ i ].dblTimes	= pow( 2, nShift );
		_arrPowDepositTable[ i ].dblDeposit	= 50 * pow( _arrPowDepositTable[ i ].dblTimes, (double)( 1.0 / 3.0 ) );
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
 *	@param	{double}	dblDeposit
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
