/**
 * 	TrustNote Equihash PoW Miner
 *
 *
 *	Copyright @ TrustNote.com
 *
 * 	@history
 *	Created by Liu QiXing on 2018/8/22.
 */

#include <stdint.h>
#include <stddef.h>
#include "utility.h"




/**
 *	convert bytes to hex string
 *
 *	@param	{const uint8_t *}	pcuSource
 *	@param	{uint32_t}		uSourceLength
 *	@param	{char *}		pszTarget
 *	@return	{int}
 */
int bytesToHexString( const uint8_t * pcuSource, uint32_t uSourceLength, char * pszTarget )
{
	uint32_t i;
	unsigned char cHighByte;
	unsigned char cLowByte;

	if ( NULL == pcuSource )
	{
		return -1;
	}
	if ( NULL == pszTarget )
	{
		return -1;
	}
	if ( 0 == uSourceLength )
	{
		return -1;
	}

	for ( i = 0; i < uSourceLength; i++ )
	{
		cHighByte	= pcuSource[ i ] >> 4;
		cLowByte	= pcuSource[ i ] & 0x0f ;

		if ( cHighByte > 0x09 )
		{
			pszTarget[ i * 2 ] = cHighByte + 0x57;
		}
		else
		{
			pszTarget[ i * 2 ] = cHighByte | 0x30;
		}

		if ( cLowByte > 0x09 )
		{
			pszTarget[ i * 2 + 1 ] = cLowByte + 0x57;
		}
		else
		{
			pszTarget[ i * 2 + 1 ] = cLowByte | 0x30;
		}
	}

	//	...
	return 0;
}