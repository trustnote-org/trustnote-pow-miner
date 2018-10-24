//
// Created by Liu QiXing on 2018/10/24.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <algorithm>

//#include "bitcoin-consensus-params.h"
//#include "uint256.h"
//#include "arith_uint256.h"
//#include "equihash_avx2.h"
//#include "crypto/blake2b.h"
//
//#include "utility.h"


#ifdef WIN32
	#ifndef strncasecmp
		#define strncasecmp strnicmp
	#endif
#endif


void test_bitsToTarget();





int main()
{
	test_bitsToTarget();

	//	...
	return 0;
}



void test_bitsToTarget()
{
	uint32_t uBitsDec		= 528482303;	//	in decimal
	uint32_t uBitsHex		= 0x1F7FFFFF;	//	in hexadecimal
	const char * pszTarget[]	= { "007fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff" };

	//
	//	the first 1 byte means
	//		the size of the target in bytes
	//
	uint32_t uExponent		= uBitsHex >> 24;
	if ( uExponent <= 32 )
	{
		//
		//	leading zero count
		//
		uint32_t uLeadZeroCount		= 32 - uExponent;

		//	the initial 3 bytes of the target
		uint32_t uCoefficient		= ( uBitsHex << 8 ) >> 8;

		//	...
		printf( "uBitsHex\t: 0x%08X\n", uBitsHex );
		printf( "uExponent\t: 0x%08X\n", uExponent );
		printf( "uCoefficient\t: 0x%08X\n", uCoefficient );
	}
	else
	{
		printf( "uExponent must great then 32" );
	}
}













