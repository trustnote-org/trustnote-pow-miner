const _trustnote_pow_miner	= require( 'trustnote-pow-miner' );


let sTarget256Hex	= "00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff";
let nBitsUint32		= _trustnote_pow_miner.getBitsByTarget( sTarget256Hex );


console.log( `CONVERT DIFFICULT FROM 256 HEX STRING TO UINT32` );
console.log( `sTarget256Hex\t: ${ sTarget256Hex }` );
console.log( `nBitsUint32	\t: ${ nBitsUint32 }` );





