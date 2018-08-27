const _ref	= require( 'ref' );
const _ffi	= require( 'ffi' );



let _objMinerLibrary = null;



let uOutMemNonce		= _ref.alloc( _ref.types.uint );
let uActualNonce		= null;

let bufInputHeader		= new Buffer( 140 );
let uDifficulty			= 536936447;
let bufHashHex			= new Buffer( 64 );
let sActualHashHex		= null;

let nCallStartMining		= null;
let nCallCheckProofOfWork	= null;


//
//	int startMining
// 	(
// 		uint8_t * putInputHeader,
// 		uint32_t uDifficulty,
// 		uint32_t uNonceStart,
// 		uint32_t uNonceTimes,
// 		OUT uint32_t * puNonce,
// 		OUT char * pszHashHex,
// 		uint32_t uHashHexLength
// 	)
//
nCallStartMining	= _objMinerLibrary.startMining( bufInputHeader, uDifficulty, 0, 0, uOutMemNonce, bufHashHex, bufHashHex.length );
uActualNonce		= uOutMemNonce.deref();
sActualHashHex		= bufHashHex.toString();

console.log( `startMining return ${ nCallStartMining }, nonce : ${ uActualNonce }, hash : ${ sActualHashHex }` );


//
//	int checkProofOfWork
// 	(
// 		uint8_t * putInputHeader,
// 		uint32_t uDifficulty,
// 		uint32_t uNonce,
// 		const char * pcszHashHex
// 	)
//
nCallCheckProofOfWork	= _objMinerLibrary.checkProofOfWork( bufInputHeader, uDifficulty, uActualNonce, Buffer.from( sActualHashHex, 'ascii' ) );



console.log( `checkProofOfWork return ${ nCallCheckProofOfWork }` );





/**
 *	@class	CTrustMinerLibrary
 */
class CTrustMinerLibrary
{
	constructor()
	{
		if ( null === _objMinerLibrary )
		{
			_objMinerLibrary = _ffi.Library
			(
				this._getLibraryFullFilename(),
				{
					'startMining'			: [ 'int', [ 'pointer', 'uint', 'uint', 'uint', _ref.refType('uint'), 'char *', 'uint' ] ],
					'checkProofOfWork'		: [ 'int', [ 'pointer', 'uint', 'uint', 'pointer' ] ],
					'calculateNextDifficulty'	: [ 'uint', [ 'uint', 'uint', 'uint' ] ],
				}
			);
		}
	}


//	int startMining
// 	(
// 		uint8_t * putInputHeader,
// 		uint32_t uDifficulty,
// 		uint32_t uNonceStart,
// 		uint32_t uNonceTimes,
// 		OUT uint32_t * puNonce,
// 		OUT char * pszHashHex,
// 		uint32_t uHashHexLength
// 	)
//
	startMining( bufInputHeader, uDifficulty, uNonceStart, uNonceTimes, uOutMemNonce, bufHashHex, bufHashHex.length )
	{
	}


	_getLibraryFullFilename()
	{
		let sRet	= null;

		switch ( process.platform )
		{
			case 'linux' :
				sRet = `${ __dirname }/../lib/Linux/miner.so`;
				break;
			default:
				sRet = `${ __dirname }/../lib/Linux/miner.so`;
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
