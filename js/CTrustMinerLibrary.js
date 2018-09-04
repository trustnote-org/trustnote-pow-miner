'use strict';

const _fs	= require( 'fs' );
const _ref	= require( 'ref' );
const _ffi	= require( 'ffi' );


/**
 *	@variables
 */
let _objMinerLibrary = null;




/**
 *	@class	CTrustMinerLibrary
 */
class CTrustMinerLibrary
{
	constructor()
	{
		let sLibraryFullFilename	= this._getLibraryFullFilename();

		if ( null === _objMinerLibrary && sLibraryFullFilename )
		{
			_objMinerLibrary = _ffi.Library
			(
				sLibraryFullFilename,
				{
					'startMining' :
					[
						'int',
						[ 'pointer', 'uint', 'uint', 'uint', _ref.refType('uint'), 'char *', 'uint' ]
					],
					'checkProofOfWork' :
					[
						'int',
						[ 'pointer', 'uint', 'uint', 'pointer' ]
					],
					'calculateNextDifficulty' :
					[
						'uint',
						[ 'uint', 'uint', 'uint' ]
					],
					'difficulty256HexToUInt32' :
					[
						'uint',
						[ 'pointer' ]
					]
				}
			);
		}
	}


	/**
	 *	start mining
	 *
	 *	@param	{Buffer}	bufInputHeader
	 *	@param	{number}	uDifficulty
	 *	@param	{number}	uNonceStart
	 *	@param	{number}	uCalcTimes
	 *	@param	{function}	pfnCallback( err, { hashHex : '', nonce : 0 } )
	 *	@return {*}
	 *
	 * 	@description
	 *
	 * 	in c++:
	 *	int startMining
	 *	(
	 *		uint8_t * putInputHeader,
	 *		uint32_t uDifficulty,
	 *		uint32_t uNonceStart,
	 *		uint32_t uCalcTimes,
	 *		OUT uint32_t * puNonce,
	 *		OUT char * pszHashHex,
	 *		uint32_t uHashHexLength
	 *	)
	 */
	startMining( bufInputHeader, uDifficulty, uNonceStart, uCalcTimes, pfnCallback )
	{
		if ( ! _objMinerLibrary )
		{
			return pfnCallback( 'failed to load miner library.' );
		}
		if ( 'object' !== typeof bufInputHeader )
		{
			return pfnCallback( 'call startMining with invalid bufInputHeader.' );
		}
		if ( 140 !== bufInputHeader.length )
		{
			return pfnCallback( `call startMining with invalid length(${ bufInputHeader.length }) of bufInputHeader.` );
		}
		if ( 'number' !== typeof uDifficulty || uDifficulty <= 0 )
		{
			return pfnCallback( 'call startMining with invalid uDifficulty.' );
		}
		if ( 'number' !== typeof uNonceStart || uNonceStart <= 0 )
		{
			return pfnCallback( 'call startMining with invalid uNonceStart.' );
		}
		if ( 'number' !== typeof uCalcTimes || uCalcTimes <= 0 )
		{
			return pfnCallback( 'call startMining with invalid uCalcTimes.' );
		}

		let uOutMemNonce		= _ref.alloc( _ref.types.uint );
		let uActualNonce		= null;
		let bufHashHex			= new Buffer( 64 );
		let sActualHashHex		= null;

		let nCallStartMining		= _objMinerLibrary.startMining
			(
				bufInputHeader,
				uDifficulty,
				uNonceStart,
				uCalcTimes,
				uOutMemNonce,
				bufHashHex,
				bufHashHex.length
			);
		if ( 0 === nCallStartMining )
		{
			uActualNonce		= uOutMemNonce.deref();
			sActualHashHex		= bufHashHex.toString();
			return pfnCallback( null, { hashHex : sActualHashHex, nonce : uActualNonce } );
		}
		else
		{
			return pfnCallback( `startMining return error code : ${ nCallStartMining }.` );
		}
	}


	/**
	 *	check proof of work
	 *
	 *	@param	{Buffer}	bufInputHeader
	 *	@param	{number}	uDifficulty
	 *	@param	{number}	uActualNonce
	 *	@param	{string}	sActualHashHex
	 *	@param	{function}	pfnCallback( err, { code : 0 } )
	 *	@return {*}
	 *
	 * 	@description
	 *
	 *	in c++:
	 *	int checkProofOfWork
	 *	(
	 *		uint8_t * putInputHeader,
	 *		uint32_t uDifficulty,
	 *		uint32_t uNonce,
	 *		const char * pcszHashHex
	 *	)
	 *
	 * 	@callback
	 * 		code:
	 *		0	successfully
	 */
	checkProofOfWork( bufInputHeader, uDifficulty, uActualNonce, sActualHashHex, pfnCallback )
	{
		if ( ! _objMinerLibrary )
		{
			return pfnCallback( 'failed to load miner library.' );
		}
		if ( 'object' !== typeof bufInputHeader || 140 !== bufInputHeader.length )
		{
			return pfnCallback( 'call checkProofOfWork with invalid bufInputHeader.' );
		}
		if ( 'number' !== typeof uDifficulty || uDifficulty <= 0 )
		{
			return pfnCallback( 'call checkProofOfWork with invalid uDifficulty.' );
		}
		if ( 'number' !== typeof uActualNonce || uActualNonce <= 0 )
		{
			return pfnCallback( 'call checkProofOfWork with invalid uActualNonce.' );
		}

		let bufActualHashHex		= Buffer.from( sActualHashHex, 'ascii' );
		let nCallCheckProofOfWork	= _objMinerLibrary.checkProofOfWork( bufInputHeader, uDifficulty, uActualNonce, bufActualHashHex );

		if ( 0 === nCallCheckProofOfWork )
		{
			return pfnCallback( null, { code : nCallCheckProofOfWork } );
		}
		else
		{
			return pfnCallback( `failed with error code : ${ nCallCheckProofOfWork }`, { code : nCallCheckProofOfWork } );
		}
	}


	/**
	 *	calculate next cycle difficulty
	 *
	 *	@param	{number}	uPreviousDifficulty
	 *	@param	{number}	uTimeUsed
	 *	@param	{number}	uTimeStandard
	 *	@param	{function}	pfnCallback( err, { difficulty : 0 } )
	 *	@return	{*}
	 *
	 *	@description
	 *
	 * 	in c++ :
	 *	uint32_t calculateNextDifficulty
	 *		(
	 *			uint32_t uPreviousDifficulty,
	 *			uint32_t uTimeUsed,
	 *			uint32_t uTimeStandard
	 *		);
	 */
	calculateNextDifficulty( uPreviousDifficulty, uTimeUsed, uTimeStandard, pfnCallback )
	{
		if ( ! _objMinerLibrary )
		{
			return pfnCallback( 'failed to load miner library.' );
		}
		if ( 'number' !== typeof uPreviousDifficulty || uPreviousDifficulty <= 0 )
		{
			return pfnCallback( 'call calculateNextDifficulty with invalid uPreviousDifficulty.' );
		}
		if ( 'number' !== typeof uTimeUsed || uTimeUsed <= 0 )
		{
			return pfnCallback( 'call calculateNextDifficulty with invalid uTimeUsed.' );
		}
		if ( 'number' !== typeof uTimeStandard || uTimeStandard <= 0 )
		{
			return pfnCallback( 'call calculateNextDifficulty with invalid uTimeStandard.' );
		}

		let uNextDifficulty	= _objMinerLibrary.calculateNextDifficulty( uPreviousDifficulty, uTimeUsed, uTimeStandard );

		return pfnCallback( null, { difficulty : uNextDifficulty } );
	}

	/**
	 *	convert 256 bits string to uint32_t
	 *	@param	{string}	sDifficulty256Hex	hex string with length of 64
	 *	@returns {number}
	 */
	difficulty256HexToUInt32( sDifficulty256Hex )
	{
		return _objMinerLibrary.difficulty256HexToUInt32( Buffer.from( sDifficulty256Hex, 'ascii' ) );
	}


	/**
	 *	get library full filename
	 *
	 *	@private
	 *	@return {string}
	 */
	_getLibraryFullFilename()
	{
		let sRet = null;

		switch ( process.platform )
		{
			case 'linux' :
				sRet = `${ __dirname }/../lib/Linux/miner.so`;
				if ( ! _fs.existsSync( sRet ) )
				{
					sRet = null;
				}
				break;

			case 'darwin' :
				sRet = `${ __dirname }/../lib/Mac/miner.dylib`;
				if ( ! _fs.existsSync( sRet ) )
				{
					sRet = null;
				}
				break;

			default:
				break;
		}

		return sRet;
	}

}




/**
 *	@exports
 *	@type {CTrustMinerLibrary}
 */
module.exports	= CTrustMinerLibrary;
