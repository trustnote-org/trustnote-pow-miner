process.env.ENV_TRUST_MINER_DEBUG = true;

/**
 *	@modules
 */
const _miner	= require( '../js/index.js' );


let bufInputHeader	= Buffer.alloc( 140 );
let uBits		= _miner.getBitsByTarget( "0007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff" );
let nActualNonce	= 4862;
let sActualHashHex	= '000741ff9cb1081de2c66cadb75aca4183342bd288bc541d076dbceec6a9bef9';

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