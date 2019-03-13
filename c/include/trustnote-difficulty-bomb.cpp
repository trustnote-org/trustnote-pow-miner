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



/**
 *	get bomb shift value by round index
 *
 *	@param	{uint32_t}	uExplodingRoundIndex
 *	@param	{uint32_t}	uRoundIndex
 *	@return	{int}		shift a 256bit target value right
 */
int TrustNoteDifficultyBomb::getBombShiftByRoundIndex( uint32_t uExplodingRoundIndex, uint32_t uRoundIndex )
{
	//
	//	ETH :
	//	Math.pow( 2, Math.floor( block.number / 100000 ) - 2 )
	//
	double   dblValidExplodingRoundIndex	= uExplodingRoundIndex > 0 ? ( uExplodingRoundIndex / 2 ) : 0.5;
	double   dblExponent			= floor( uRoundIndex / dblValidExplodingRoundIndex ) - 2.0;
	double   dblPow				= pow( 2, dblExponent );
	uint32_t uShift				= dblPow >= 256.0 ? 256 : (uint32_t)dblPow;

	#ifdef _DEBUG_BOMB
		printf( "TrustNoteDifficultyBomb::getBombShiftByRoundIndex( %u, %u );\n", uExplodingRoundIndex, uRoundIndex );
		printf( "- dblValidExplodingRoundIndex\t: %f\n", dblValidExplodingRoundIndex );
		printf( "- dblExponent\t\t\t: %f\n", dblExponent );
		printf( "- dblPow\t\t\t: %f\n", dblPow );
		printf( "- uShift\t\t\t: -1 * %u\n", uShift );
		printf( "\n" );

	#endif

	//	...
	return -1 * uShift;
}


