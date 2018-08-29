const _miner	= require( '../js/index.js' );




/**
 *	stop
 */
let nDifficulty = _miner.difficulty256HexToUInt32( "00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff" );
console.log( `nDifficulty value : ${ nDifficulty }` );
