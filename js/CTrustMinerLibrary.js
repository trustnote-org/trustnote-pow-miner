'use strict';

const _fs	= require( 'fs' );
const _ref	= require( 'ref' );
const _ffi	= require( 'ffi' );


/**
 *	@variables
 */
let _objMinerLibrary	= null;




/**
 *	@class	CTrustMinerLibrary
 */
class CTrustMinerLibrary
{
	constructor()
	{
		//
		//	set process.env.ENV_TRUST_MINER_DEBUG = true to load libraries with debug info
		//
		this._loadLibrary();
	}

	/**
	 *	start mining
	 *
	 *	@param	{Buffer}	bufInputHeader
	 *	@param	{number}	uBits
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
	 *		uint8_t * pcutInputHeader,
	 *		uint32_t uBits,
	 *		uint32_t uNonceStart,
	 *		uint32_t uCalcTimes,
	 *		OUT uint32_t * puNonce,
	 *		OUT char * pszHashHex,
	 *		uint32_t uHashHexLength
	 *	)
	 */
	startMining( bufInputHeader, uBits, uNonceStart, uCalcTimes, pfnCallback )
	{
		if ( ! _objMinerLibrary )
		{
			return pfnCallback( 'failed to load miner library.' );
		}
		if ( null === bufInputHeader || 'object' !== typeof bufInputHeader )
		{
			return pfnCallback( 'call startMining with invalid bufInputHeader.' );
		}
		if ( 140 !== bufInputHeader.length )
		{
			return pfnCallback( `call startMining with invalid length(${ bufInputHeader.length }) of bufInputHeader.` );
		}
		if ( 'number' !== typeof uBits || uBits < 0 )
		{
			return pfnCallback( 'call startMining with invalid uBits.' );
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
		let bufHashHex			= Buffer.alloc( 64 );
		let sActualHashHex		= null;

		let nCallStartMining		= _objMinerLibrary.startMining
			(
				bufInputHeader,
				uBits,
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
			return pfnCallback( `call startMining unsuccessfully with code: ${ nCallStartMining }.` );
		}
	}


	/**
	 *	check proof of work
	 *
	 *	@param	{Buffer}	bufInputHeader
	 *	@param	{number}	uBits
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
	 *		uint8_t * pcutInputHeader,
	 *		uint32_t uBits,
	 *		uint32_t uNonce,
	 *		const char * pcszHashHex
	 *	)
	 *
	 * 	@callback
	 * 		code:
	 *		0	successfully
	 */
	checkProofOfWork( bufInputHeader, uBits, uActualNonce, sActualHashHex, pfnCallback )
	{
		if ( ! _objMinerLibrary )
		{
			return pfnCallback( 'failed to load miner library.' );
		}
		if ( null === bufInputHeader || 'object' !== typeof bufInputHeader || 140 !== bufInputHeader.length )
		{
			return pfnCallback( 'call checkProofOfWork with invalid bufInputHeader.' );
		}
		if ( 'number' !== typeof uBits || uBits < 0 )
		{
			return pfnCallback( 'call checkProofOfWork with invalid uBits.' );
		}
		if ( 'number' !== typeof uActualNonce || uActualNonce <= 0 )
		{
			return pfnCallback( 'call checkProofOfWork with invalid uActualNonce.' );
		}

		let bufActualHashHex		= Buffer.from( sActualHashHex, 'ascii' );
		let nCallCheckProofOfWork	= _objMinerLibrary.checkProofOfWork( bufInputHeader, uBits, uActualNonce, bufActualHashHex );

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
	 *	calculate next cycle bits
	 *
	 *	@param	{number}	uPreviousBits
	 *	@param	{number}	uTimeUsed
	 *	@param	{number}	uTimeStandard
	 *	@param	{function}	pfnCallback( err, { bits : 0 } )
	 *	@return	{*}
	 *
	 *	@description
	 *
	 * 	in c++ :
	 *	uint32_t calculateNextWorkRequired
	 *		(
	 *			uint32_t uPreviousBits,
	 *			uint32_t uTimeUsed,
	 *			uint32_t uTimeStandard
	 *		);
	 */
	calculateNextWorkRequired( uPreviousBits, uTimeUsed, uTimeStandard, pfnCallback )
	{
		if ( ! _objMinerLibrary )
		{
			return pfnCallback( 'failed to load miner library.' );
		}
		if ( 'number' !== typeof uPreviousBits || uPreviousBits <= 0 )
		{
			return pfnCallback( 'call calculateNextWorkRequired with invalid uPreviousBits.' );
		}
		if ( 'number' !== typeof uTimeUsed || uTimeUsed <= 0 )
		{
			return pfnCallback( 'call calculateNextWorkRequired with invalid uTimeUsed.' );
		}
		if ( 'number' !== typeof uTimeStandard || uTimeStandard <= 0 )
		{
			return pfnCallback( 'call calculateNextWorkRequired with invalid uTimeStandard.' );
		}

		let uNextBits	= _objMinerLibrary.calculateNextWorkRequired( uPreviousBits, uTimeUsed, uTimeStandard );

		return pfnCallback( null, { bits : uNextBits } );
	}

	/**
	 *	calculate next work required target in 32 bits format with deposit and round index
	 *
	 *	@param	{number}	uPreviousBits		- unsigned int
	 *	@param	{number}	uTimeUsed		- unsigned int
	 *	@param	{number}	uTimeStandard		- unsigned int
	 *	@param	{number}	dblDeposit		- double
	 *	@param	{number}	uRoundIndex		- unsigned int
	 *	@param	{function}	pfnCallback( err, { bits : 0, shiftByDeposit : 0, shiftByRoundIndex : 0 } )
	 *	@return	{*}
	 *
	 *	@description
	 *
	 * 	in c++ :
	 *	uint32_t calculateNextWorkRequiredWithDeposit
	 *		(
	 *			const uint32_t uPreviousBits,
	 *			const uint32_t uTimeUsed,
	 *			const uint32_t uTimeStandard,
	 *			const double   dblDeposit,
	 *			const uint32_t uRoundIndex
	 *		)
	 */
	calculateNextWorkRequiredWithDeposit( uPreviousBits, uTimeUsed, uTimeStandard, dblDeposit, uRoundIndex, pfnCallback )
	{
		if ( ! _objMinerLibrary )
		{
			return pfnCallback( 'failed to load miner library.' );
		}
		if ( 'number' !== typeof uPreviousBits || uPreviousBits <= 0 )
		{
			return pfnCallback( 'call calculateNextWorkRequired with invalid uPreviousBits.' );
		}
		if ( 'number' !== typeof uTimeUsed || uTimeUsed <= 0 )
		{
			return pfnCallback( 'call calculateNextWorkRequired with invalid uTimeUsed.' );
		}
		if ( 'number' !== typeof uTimeStandard || uTimeStandard <= 0 )
		{
			return pfnCallback( 'call calculateNextWorkRequired with invalid uTimeStandard.' );
		}
		if ( 'number' !== typeof dblDeposit )
		{
			return pfnCallback( 'call calculateNextWorkRequired with invalid dblDeposit.' );
		}
		if ( 'number' !== typeof uRoundIndex || uRoundIndex < 0 )
		{
			return pfnCallback( 'call calculateNextWorkRequired with invalid uRoundIndex.' );
		}

		//	...
		let nShiftByDeposit	= _objMinerLibrary.calculateShiftByDeposit( dblDeposit );
		let nShiftByRoundIndex	= _objMinerLibrary.calculateShiftByRoundIndex( uRoundIndex );
		let uNextBits		= _objMinerLibrary.calculateNextWorkRequiredWithDeposit( uPreviousBits, uTimeUsed, uTimeStandard, dblDeposit, uRoundIndex );

		return pfnCallback
		(
			null,
			{
				bits			: uNextBits,
				shiftByDeposit		: nShiftByDeposit,
				shiftByRoundIndex	: nShiftByRoundIndex,
			}
		);
	}


	/**
	 *	convert 256 bits string to uint32_t
	 *	@param	{string}	sDifficulty256Hex	hex string with length of 64
	 *	@returns {number}
	 */
	getBitsByTarget( sDifficulty256Hex )
	{
		if ( ! _objMinerLibrary )
		{
			return 0;
		}
		return _objMinerLibrary.getBitsByTarget( Buffer.from( sDifficulty256Hex, 'ascii' ) );
	}


	/**
	 *	convert 32 bits uint32_t bits to 256 bits string target
	 *
	 * 	@param	{number}	uBits
	 *
	 * 	@description
	 *
	 * 	in c++
	 *
	 * 	EXPORT_API int getTargetByBits( uint32_t uBits, OUT char * pszTargetHex, uint32_t uSize );
	 *	@param 	{uint32_t}	uBits		e.g.: 0x1c03a809
	 *	@param 	{char *}	pszTargetHex	e.g.: "00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
	 *	@param 	{uint32_t}	uSize
	 *	@return	{int}
	 */
	getTargetByBits( uBits, pfnCallback )
	{
		if ( ! _objMinerLibrary )
		{
			return pfnCallback( 'failed to load miner library.' );
		}
		if ( 'number' !== typeof uBits || uBits < 0 )
		{
			return pfnCallback( 'call startMining with invalid uBits.' );
		}

		let bufTargetHex	= Buffer.alloc( 64 );
		let nCall		= _objMinerLibrary.getTargetByBits
		(
			uBits,
			bufTargetHex,
			bufTargetHex.length
		);
		if ( 0 === nCall )
		{
			return pfnCallback( null, bufTargetHex.toString() );
		}
		else
		{
			return pfnCallback( `call getTargetByBits unsuccessfully with code: ${ nCall }.` );
		}
	}


	/**
	 *	is currently in debug model
	 *
	 *	@private
	 *	@return {boolean}
	 */
	_isDebugEnv()
	{
		return ( process.env &&
			'object' === typeof process.env &&
			'string' === typeof process.env.ENV_TRUST_MINER_DEBUG &&
			'true' === process.env.ENV_TRUST_MINER_DEBUG.toLowerCase() );
	}

	/**
	 *	get library full filename
	 *
	 *	@private
	 *	@return {string}
	 */
	_getLibraryFullFilename()
	{
		let sRet 	= null;
		let bDebug	= this._isDebugEnv();

		switch ( process.platform )
		{
			case 'linux' :
				sRet = `${ __dirname }/../lib/Linux/${ bDebug ? 'miner.Linux.debug.so' : 'miner.Linux.release.so' }`;
				if ( ! _fs.existsSync( sRet ) )
				{
					sRet = null;
				}
				break;

			case 'darwin' :
				sRet = `${ __dirname }/../lib/Mac/${ bDebug ? 'miner.Mac.debug.dylib' : 'miner.Mac.release.dylib' }`;
				if ( ! _fs.existsSync( sRet ) )
				{
					sRet = null;
				}
				break;

			case 'win32' :
				sRet = `${ __dirname }/../lib/Windows/${ bDebug ? 'miner.Windows.x64.debug.dll' : 'miner.Windows.x64.release.dll' }`;
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

	/**
	 *	load library
	 *
	 *	@return	{boolean}
	 */
	_loadLibrary()
	{
		if ( _objMinerLibrary )
		{
			return true;
		}

		let sLibraryFullFilename = this._getLibraryFullFilename();
		if ( sLibraryFullFilename )
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
					'calculateNextWorkRequired' :
						[
							'uint',
							[ 'uint', 'uint', 'uint' ]
						],
					'calculateNextWorkRequiredWithDeposit' :
						[
							'uint',
                            				[ 'uint', 'uint', 'uint', 'double', 'uint' ]
						],
					'calculateShiftByDeposit' :
						[
							'int',
							[ 'double' ]
						],
					'calculateShiftByRoundIndex' :
						[
							'int',
							[ 'uint' ]
						],
					'getBitsByTarget' :
						[
							'uint',
							[ 'pointer' ]
						],
					'getTargetByBits' :
						[
							'int',
							[ 'uint', 'char *', 'uint' ]
						]
				}
			);
		}

		return !! _objMinerLibrary;
	}

}




/**
 *	@exports
 *	@type {CTrustMinerLibrary}
 */
module.exports	= CTrustMinerLibrary;
