process.env.ENV_TRUST_MINER_DEBUG = true;

/**
 *	@modules
 */
const _miner	= require( '../js/index.js' );


//let bufInputHeader	= Buffer.alloc( 140 );
let sInputHeaderHex		= 'f93c6f10ded14bc3d2351389bdf8aa50aed5e1ea2182cc9e031a2c75f05ccec9ac96251ec756ad5dfc75935f05a02f5f48cdf5ad0a090830dbb78cbebb198d664d66fd93d3c04184af48a5ad093a9f79fdb07888875a1d3b0aab54993becd9edc75ef252447cb2da6b99c9df021007cb06463a97382c4776dfa76aa34dbc5c39d67428e4fc9dc99cb736db4b';
let bufInputHeader		= Buffer.from( sInputHeaderHex, 'hex' );


let uBits		= 528548564;	//_miner.getBitsByTarget( "007fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff" );
let nActualNonce	= 1573;
let sActualHashHex	= '008b9d227adec19f9c954d45d786c81d109b6c523900613d9bc7f257419ecade';

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
