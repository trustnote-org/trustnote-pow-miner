const _ref	= require( 'ref' );
const _ffi	= require( 'ffi' );

let _objMinerLibrary = null;


_objMinerLibrary = _ffi.Library
(
	`${ __dirname }/miner.x64.debug.dll`,
	{
		'startMining'			: [ 'int', [ 'pointer', 'uint', 'uint', 'uint', _ref.refType('uint'), 'char *', 'uint' ] ],
		'checkProofOfWork'		: [ 'int', [ 'pointer', 'uint', 'uint', 'pointer' ] ],
		'calculateNextWorkRequired'	: [ 'uint', [ 'uint', 'uint', 'uint' ] ],
	}
);

let uOutMemNonce		= _ref.alloc( _ref.types.uint );
let uActualNonce		= null;

let bufInputHeader		= Buffer.alloc( 140 );
let uBits			= 536936447;
let bufHashHex			= Buffer.alloc( 64 );
let sActualHashHex		= null;

let nCallStartMining		= null;
let nCallCheckProofOfWork	= null;


//
//	int startMining
// 	(
// 		uint8_t * pcutInputHeader,
// 		uint32_t uBits,
// 		uint32_t uNonceStart,
// 		uint32_t uCalcTimes,
// 		OUT uint32_t * puNonce,
// 		OUT char * pszHashHex,
// 		uint32_t uHashHexLength
// 	)
//
nCallStartMining	= _objMinerLibrary.startMining( bufInputHeader, uBits, 0, 0, uOutMemNonce, bufHashHex, bufHashHex.length );
uActualNonce		= uOutMemNonce.deref();
sActualHashHex		= bufHashHex.toString();

console.log( `startMining return ${ nCallStartMining }, nonce : ${ uActualNonce }, hash : ${ sActualHashHex }` );


//
//	int checkProofOfWork
// 	(
// 		uint8_t * pcutInputHeader,
// 		uint32_t uBits,
// 		uint32_t uNonce,
// 		const char * pcszHashHex
// 	)
//
nCallCheckProofOfWork	= _objMinerLibrary.checkProofOfWork( bufInputHeader, uBits, uActualNonce, Buffer.from( sActualHashHex, 'ascii' ) );



console.log( `checkProofOfWork return ${ nCallCheckProofOfWork }` );



//
//	uint32_t calculateNextWorkRequired
// 	(
// 		uint32_t uPreviousBits,
// 		uint32_t uTimeUsed,
// 		uint32_t uTimeStandard
// 	);
//
let arrDifficultyInputs	=
	[
		[ uBits, 15000, 15000, 0 ],
		[ uBits, 10000, 15000, 0 ],
		[ uBits, 19000, 15000, 0 ],
		[ uBits, 30000, 15000, 0 ],
		[ uBits,  5000, 15000, 0 ],
	];
for ( let i = 0; i < arrDifficultyInputs.length; i ++ )
{
	arrDifficultyInputs[ i ][ 3 ]	= _objMinerLibrary.calculateNextWorkRequired
	(
		arrDifficultyInputs[ i ][ 0 ],
		arrDifficultyInputs[ i ][ 1 ],
		arrDifficultyInputs[ i ][ 2 ]
	);
}

console.log( `result of calculateNextWorkRequired : \n`, arrDifficultyInputs );
