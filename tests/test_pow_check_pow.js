process.env.ENV_TRUST_MINER_DEBUG = true;

/**
 *	@modules
 */
const _miner	= require( '../js/index.js' );


//let bufInputHeader	= Buffer.alloc( 140 );
let sInputHeaderHex		= '53361928fa83a18646fcee23e66750b336d61129d5e71352b8032c8acaf67da3e793c4cb782712d9a13162e10e7080d966ff785abf708d15f5b01848f90c74764e6b65dae2ec03bcab28531b56786d2bd102ae608f8216d7ad3246111f67960e00a23680e668ba7701e0d0e0ce3a91ac907b1695bb154838c477623c0468b364c4c68ac18cddc846e53207e1';
let bufInputHeader		= Buffer.from( sInputHeaderHex, 'hex' );


let uBits		= _miner.getBitsByTarget( "0087974bffffffffffffffffffffffffffffffffffffffffffffffffffffffff" );
let nActualNonce	= 94;
let sActualHashHex	= '006de085e610fae322963cf8e1983094f9ba94a632b8bee47fe0bfd81d463aae';

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