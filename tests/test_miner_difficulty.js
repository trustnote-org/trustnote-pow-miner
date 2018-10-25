process.env.ENV_TRUST_MINER_DEBUG = true;


const _miner	= require( '../js/index.js' );


let nDifficulty = _miner.getBitsByTarget( "007fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff" );
console.log( `nDifficulty value : ${ nDifficulty }` );



//
//	calculate next bits
//
let nAverageBits	= nDifficulty;
let nTimeUsed		= 1000;
let nTimeStandard	= 2400;
_miner.calculateNextWorkRequired
(
	nAverageBits,
	nTimeUsed,
	nTimeStandard,
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

		if ( oData &&
			'object' === typeof oData )
		{
			if ( oData.hasOwnProperty( 'bits' ) &&
				'number' === typeof oData.bits &&
				oData.bits > 0 )
			{
				console.log( `new bits value after ${ nAverageBits } is : ${ oData.bits }` );
			}
			else
			{
				console.log( `calculateNextWorkRequired callback :: invalid value .bits` );
			}
		}
		else
		{
			console.log( `calculateNextWorkRequired callback :: invalid oData object` );
		}
	}
);