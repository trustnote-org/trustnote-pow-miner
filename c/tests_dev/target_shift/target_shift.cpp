//
// Created by Liu QiXing on 2018/11/02.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <algorithm>
#include <iostream>
#include <cstdlib>

//#include "bitcoin-consensus-params.h"
#include "uint256.h"
#include "arith_uint256.h"


#ifndef IN
	#define IN
#endif
#ifndef OUT
	#define OUT
#endif



void test_shift();
uint32_t getExponentOfBits( uint32_t uBits );
int getTargetByBits( uint32_t uBits, OUT char * pszTargetHex, uint32_t uSize );




int main()
{
	test_shift();

	//	...
	return 0;
}



void test_shift()
{
	const uint32_t uMaxShift	= 256;
	arith_uint256 bnNormalUInt256;
	char szTarget[ 66 ];
	uint32_t i;
	uint32_t uExponent;
	uint32_t uBits;

	//
	//	shift right
	//
	bnNormalUInt256	= UintToArith256( uint256S( "0007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff" ) );

	printf( "\n>>=\n" );
	for ( i = 0; i <= uMaxShift; i ++ )
	{
		//	...
		bnNormalUInt256 >>= 1;

		//	...
		uBits	= bnNormalUInt256.GetCompact();

		memset( szTarget, 0, sizeof( szTarget ) );
		getTargetByBits( uBits, szTarget, sizeof( szTarget ) );

		printf( "%*d, ", 3, i );
		printf( "0x%08X, ", uBits );
		printf( "%*d, ", 10, uBits );
		printf( "%s\n", szTarget );
	}



	//
	//	shift right
	//
	bnNormalUInt256	= UintToArith256( uint256S( "00000000000000000000000fffffffffffffffffffffffffffffffffffffffff" ) );

	printf( "\n<<=\n" );
	for ( i = 0; i <= uMaxShift; i ++ )
	{
		uBits		= bnNormalUInt256.GetCompact();
		uExponent	= getExponentOfBits( uBits );

		//	...
		if ( uExponent < 32 )
		{
			bnNormalUInt256 <<= 1;
		}
		else
		{
			printf( "[0x%08X]ALREADY MAX VALUE.\n", uBits );
		}

		//	...
		uBits	= bnNormalUInt256.GetCompact();

		memset( szTarget, 0, sizeof( szTarget ) );
		getTargetByBits( uBits, szTarget, sizeof( szTarget ) );

		printf( "%*d, ", 3, i );
		printf( "0x%08X, ", uBits );
		printf( "%*d, ", 10, uBits );
		printf( "%s\n", szTarget );
	}
}


uint32_t getExponentOfBits( uint32_t uBits )
{
	return ( uBits >> 24 );
}

/**
 *	convert 32 bits uint32_t bits to 256 bits string target
 *
 *	@param 	{uint32_t}	uBits
 *	@param 	{char *}	pszTargetHex	"00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
 *	@param 	{uint32_t}	uSize
 *	@return	{int}
 */
int getTargetByBits( uint32_t uBits, OUT char * pszTargetHex, uint32_t uSize )
{
	if ( NULL == pszTargetHex )
	{
#ifdef _DEBUG
		printf( "getTargetByBits :: invalid parameter pszTargetHex, NULL.\n" );
#endif
		return -1000;
	}
	if ( uSize < 64 )
	{
#ifdef _DEBUG
		printf( "getTargetByBits :: invalid parameter uSize, must be great or equal to 64.\n" );
#endif
		return -1001;
	}

	//	...
	int nRet = -1;

	//
	//	About bits
	//	@see	http://learnmeabitcoin.com/glossary/bits
	//
	//	bits			: 0x1C033AB2
	//	exponent		: 0x1C
	//	coefficient		:   0x035DB8
	//	leading zero in bytes	: 32 - exponent
	//				  32 - 28 = 4
	//
	//	that means:
	//	target			: 00000000 035DB8 FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
	//

	//
	//	the first 1 byte means
	//		the size of the target in bytes
	//
	uint32_t uExponent		= uBits >> 24;
	if ( uExponent <= 32 )
	{
		//
		//	leading zero count in bytes
		//
		uint32_t uLeadingZeroBytes	= 32 - uExponent;

		//
		//	the initial 3 bytes of the target
		//
		uint32_t uCoefficient		= ( uBits << 8 ) >> 8;

		//	...
		memset( pszTargetHex, 0, uSize );
		sprintf( pszTargetHex, "%.*x", uLeadingZeroBytes * 2, 0 );
		sprintf( pszTargetHex + uLeadingZeroBytes * 2, "%06x", uCoefficient );
		sprintf( pszTargetHex + uLeadingZeroBytes * 2 + 6, "%*c", ( uExponent - 3 ) * 2, 'f' );

		for ( uint32_t i = 0; i < uSize; i ++ )
		{
			if ( ' ' == pszTargetHex[ i ] )
			{
				pszTargetHex[ i ] = 'f';
			}
		}

		pszTargetHex[ 64 ] = 0;

		//
		//	successfully
		//
		nRet = 0;
	}
	else
	{
		nRet = -2000;
#ifdef _DEBUG
		printf( "getTargetByBits :: uExponent must less then or equal to 32.\n" );
#endif
	}

	return nRet;
}











