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
 *	@param	{int}	nRoundIndex
 *	@return	{int}	shift a 256bit target value right
 */
int TrustNoteDifficultyBomb::getBombShiftByRoundIndex( int nRoundIndex )
{
	//	Math.pow( 2, Math.floor( block.number / 100000 ) - 2 )
	return -1 *
		(
			std::min
			(
				(int)
				(
					pow
					(
						2,
						floor( nRoundIndex / 100000 ) - 2.0
					)
				),
				256
			)
		);
}


