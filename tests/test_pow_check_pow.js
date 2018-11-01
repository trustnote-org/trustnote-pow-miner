process.env.ENV_TRUST_MINER_DEBUG = true;

/**
 *	@modules
 */
const _miner	= require( '../js/index.js' );


let bufInputHeader	= Buffer.alloc( 140 );
let uBits		= _miner.getBitsByTarget( "0007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff" );
let nActualNonce	= 91;
let sActualHashHex	= '00018033558564425fd808265ad06fd53e242a61b2f90ae4f0b94e4f4c23a99c';

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