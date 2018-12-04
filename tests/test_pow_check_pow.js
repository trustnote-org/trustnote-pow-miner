process.env.ENV_TRUST_MINER_DEBUG = true;

/**
 *	@modules
 */
const _miner	= require( '../js/index.js' );


//let bufInputHeader	= Buffer.alloc( 140 );
let sInputHeaderHex		= '0e8d2f065b670eed8300e6c199ecb7ede00934c62fe0c18db0b345849623a0b64d8ca4916eed6e0417c678e56203aaaae986563402e45ca54bb394cbdf09bc8820953b223e7323e18e91d2c34ae8387daacd906116f8f9ff2e4e6248da78f7c1f31acce34fb4a9b60abd4f5753076ead38ac064d871fda92affcd6a3785a4d847c83d0965ddb43604384a103';
let bufInputHeader		= Buffer.from( sInputHeaderHex, 'hex' );


let uBits		= 523973461;	//_miner.getBitsByTarget( "007fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff" );
let nActualNonce	= 54;
let sActualHashHex	= '00082504848696bddcead41792d1695e7066af24434ab885f1ad40f06e42a7ef';

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

