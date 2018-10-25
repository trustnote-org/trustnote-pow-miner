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


void bits32ToTarget256Hex();





int main()
{
	bits32ToTarget256Hex();

	//	...
	return 0;
}



void bits32ToTarget256Hex()
{
	//uint32_t uBitsDec		= 528482303;	//	in decimal
	//const char * pszTarget[]	= { "007fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff" };
	uint32_t uBitsHex		= 0x1c035db8;	//0x1F7FFFFF;	//	in hexadecimal
	char szTarget[ 66 ];

	//
	//	the first 1 byte means
	//		the size of the target in bytes
	//
	uint32_t uExponent		= uBitsHex >> 24;
	if ( uExponent <= 32 )
	{
		//
		//	leading zero count in bytes
		//
		uint32_t uLeadingZeroBytes	= 32 - uExponent;

		//
		//	the initial 3 bytes of the target
		//
		uint32_t uCoefficient		= ( uBitsHex << 8 ) >> 8;

		//	...
		printf( "* INPUT\n" );
		printf( "\tBits value :\n\t0x%08X, %d\n\n", uBitsHex, uBitsHex );

		printf( "* OUTPUT\n" );
		printf( "\tExponent :\n\t0x%08X, %d\n\n", uExponent, uExponent );
		printf( "\tCoefficient :\n\t0x%08X, %d\n\n", uCoefficient, uCoefficient );
		printf( "\tLeadingZeroInBytes :\n\t0x%08X, %d\n\n", uLeadingZeroBytes, uLeadingZeroBytes );

		memset( szTarget, 0, sizeof( szTarget ) );
		sprintf( szTarget, "%.*X", uLeadingZeroBytes * 2, 0 );
		sprintf( szTarget + uLeadingZeroBytes * 2, "%06X", uCoefficient );
		sprintf( szTarget + uLeadingZeroBytes * 2 + 6, "%*c", ( uExponent - 3 ) * 2, 'F' );

		for ( uint32_t i = 0; i < sizeof( szTarget ) / sizeof( szTarget[ 0 ] ); i ++ )
		{
			if ( ' ' == szTarget[ i ] )
			{
				szTarget[ i ] = 'F';
			}
		}

		printf( "\tTarget :\n\t%s\n\n\n", szTarget );
	}
	else
	{
		printf( "uExponent must less then or equal to 32\n" );
	}
}













