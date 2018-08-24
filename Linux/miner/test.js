const _ref	= require( 'ref' );
const _ffi	= require( 'ffi' );

let _objMinerLibrary = null;


_objMinerLibrary = _ffi.Library
(
	`${ __dirname }/miner.so`,
	{
		'startMining'			: [ 'int', [ 'pointer', 'uint', 'uint', 'uint', _ref.refType('uint'), 'char *', 'uint' ] ],
		'checkProofOfWork'		: [ 'int', [ 'pointer', 'uint', 'uint', 'pointer' ] ],
		'calculateNextDifficulty'	: [ 'uint', [ 'uint', 'uint', 'uint' ] ],
	}
);

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

console.log( `length of sActualHashHex : ${ Buffer.from( sActualHashHex ).length }` );


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


console.log( `startMining return ${ nCallStartMining }, nonce : ${ uActualNonce }, hash : ${ sActualHashHex }` );
console.log( `checkProofOfWork return ${ nCallCheckProofOfWork }` );



//
//	uint32_t calculateNextDifficulty
// 	(
// 		uint32_t uPreviousDifficulty,
// 		uint32_t uTimeUsed,
// 		uint32_t uTimeStandard
// 	);
//
let arrDifficultyInputs	=
	[
		[ uDifficulty, 15000, 15000, 0 ],
		[ uDifficulty, 10000, 15000, 0 ],
		[ uDifficulty, 19000, 15000, 0 ],
		[ uDifficulty, 30000, 15000, 0 ],
		[ uDifficulty,  5000, 15000, 0 ],
	];
for ( let i = 0; i < arrDifficultyInputs.length; i ++ )
{
	arrDifficultyInputs[ i ][ 3 ]	= _objMinerLibrary.calculateNextDifficulty
	(
		arrDifficultyInputs[ i ][ 0 ],
		arrDifficultyInputs[ i ][ 1 ],
		arrDifficultyInputs[ i ][ 2 ]
	);
}

console.log( `result of calculateNextDifficulty : \n`, arrDifficultyInputs );









