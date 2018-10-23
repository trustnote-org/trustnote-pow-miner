const _trustnote_pow_miner	= require( 'trustnote-pow-miner' );


let sDifficulty256Hex	= "00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff";
let nDifficultyU32	= _trustnote_pow_miner.target256HexToBits32( sDifficulty256Hex );


console.log( `CONVERT DIFFICULT FROM 256 HEX STRING TO UINT32` );
console.log( `sDifficulty256Hex\t: ${ sDifficulty256Hex }` );
console.log( `nDifficultyU32	\t: ${ nDifficultyU32 }` );





