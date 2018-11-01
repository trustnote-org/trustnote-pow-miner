process.env.ENV_TRUST_MINER_DEBUG = true;

/**
 *	@modules
 */
const _miner	= require( '../js/index.js' );


let bufInputHeader	= Buffer.alloc( 140 );
let uBits		= _miner.getBitsByTarget( "00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff" );
let nActualNonce	= 65;
let sActualHashHex	= '00198bb0606e5a8b5d47577bc96de488116af886815f4dccc5ad1ebd78d1b14e';

_miner.checkProofOfWork
(
	bufInputHeader,
	uBits,
	nActualNonce,
	sActualHashHex,
	function( err, oResult )
	{
		console.log( `######## : `, err, `oResult : `, oResult );
	}
);