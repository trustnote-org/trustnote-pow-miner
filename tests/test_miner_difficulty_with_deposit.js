process.env.ENV_TRUST_MINER_DEBUG = true;


const _miner	= require( '../js/index.js' );


let uBits = _miner.getBitsByTarget( "000007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffff" );
console.log( `uBits value : ${ uBits }` );



//
//	calculate next bits
//
let nAverageBits	= uBits;
let nTimeUsed		= 1000;
let nTimeStandard	= 2400;
_miner.calculateNextWorkRequiredWithDeposit
(
	nAverageBits,
	nTimeUsed,
	nTimeStandard,
	1000,
	600000,
	function( err, oData )
	{
		//
		//	oData
		//	{ bits : uNextBits }
		//
		if ( err )
		{
			return console.error( err );
		}

		console.log( oData );
	}
);