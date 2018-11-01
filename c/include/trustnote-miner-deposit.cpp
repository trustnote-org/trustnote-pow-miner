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
	//  nShift          		dblTimes         dblDeposit
	//
	{        0,                     0.000000,          0.000000 },
	{        1,                     1.000000,          3.000000 },
	{        2,                     3.000000,          3.820216 },
	{        3,                     7.000000,          4.603020 },
	{        4,                    15.000000,          5.443288 },
	{        5,                    31.000000,          6.385881 },
	{        6,                    63.000000,          7.464078 },
	{        7,                   127.000000,          8.708795 },
	{        8,                   255.000000,         10.152198 },
	{        9,                   511.000000,         11.829703 },
	{       10,                  1023.000000,         13.781418 },
	{       11,                  2047.000000,         16.053406 },
	{       12,                  4095.000000,         18.698945 },
	{       13,                  8191.000000,         21.779875 },
	{       14,                 16383.000000,         25.368092 },
	{       15,                 32767.000000,         29.547268 },
	{       16,                 65535.000000,         34.414810 },
	{       17,                131071.000000,         40.084153 },
	{       18,                262143.000000,         46.687398 },
	{       19,                524287.000000,         54.378404 },
	{       20,               1048575.000000,         63.336366 },
	{       21,               2097151.000000,         73.770001 },
	{       22,               4194303.000000,         85.922402 },
	{       23,               8388607.000000,        100.076710 },
	{       24,              16777215.000000,        116.562707 },
	{       25,              33554431.000000,        135.764501 },
	{       26,              67108863.000000,        158.129475 },
	{       27,             134217727.000000,        184.178711 },
	{       28,             268435455.000000,        214.519130 },
	{       29,             536870911.000000,        249.857636 },
	{       30,            1073741823.000000,        291.017581 },
	{       31,            2147483647.000000,        338.957951 },
	{       32,            4294967295.000000,        394.795709 },
	{       33,            8589934591.000000,        459.831823 },
	{       34,           17179869183.000000,        535.581568 },
	{       35,           34359738367.000000,        623.809840 },
	{       36,           68719476735.000000,        726.572273 },
	{       37,          137438953471.000000,        846.263129 },
	{       38,          274877906943.000000,        985.671089 },
	{       39,          549755813887.000000,       1148.044223 },
	{       40,         1099511627775.000000,       1337.165665 },
	{       41,         2199023255551.000000,       1557.441761 },
	{       42,         4398046511103.000000,       1814.004728 },
	{       43,         8796093022207.000000,       2112.832233 },
	{       44,        17592186044415.000000,       2460.886664 },
	{       45,        35184372088831.000000,       2866.277350 },
	{       46,        70368744177663.000000,       3338.449498 },
	{       47,       140737488355327.000000,       3888.404257 },
	{       48,       281474976710655.000000,       4528.955036 },
	{       49,       562949953421311.000000,       5275.026041 },
	{       50,      1125899906842623.000000,       6144.000000 },
	{       51,      2251799813685247.000000,       7156.123155 },
	{       52,      4503599627370495.000000,       8334.976988 },
	{       53,      9007199254740991.000000,       9708.027640 },
	{       54,     18014398509481984.000000,      11307.265851 },
	{       55,     36028797018963968.000000,      13169.952308 },
	{       56,     72057594037927936.000000,      15339.485785 },
	{       57,    144115188075855872.000000,      17866.414293 },
	{       58,    288230376151711744.000000,      20809.612797 },
	{       59,    576460752303423488.000000,      24237.654946 },
	{       60,   1152921504606846976.000000,      28230.410772 },
	{       61,   2305843009213693952.000000,      32880.907586 },
	{       62,   4611686018427387904.000000,      38297.497419 },
	{       63,   9223372036854775808.000000,      44606.381522 },
	{       64,  18446744073709551616.000000,      51954.550730 }
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
		_arrPowDepositTable[ i ].dblTimes	= pow( 2, nShift ) - 1;
		_arrPowDepositTable[ i ].dblDeposit	= 10000.0 * 1000.0 * 1000.0 * ( 3 * pow( _arrPowDepositTable[ i ].dblTimes, 0.22 ) );

		if ( _arrPowDepositTable[ i ].dblDeposit > TRUSTNOTE_MINER_DEPOSIT_MAX_NOTES )
		{
			break;
		}
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
