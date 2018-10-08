process.env.ENV_TRUST_MINER_DEBUG = true;


const _miner	= require( '../js/index.js' );


let nDifficulty = _miner.difficulty256HexToUInt32( "007fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff" );
console.log( `nDifficulty value : ${ nDifficulty }` );



//
//	calculate next difficulty
//
let nAverageDifficulty	= nDifficulty;
let nTimeUsed		= 1000;
let nTimeStandard	= 2400;
_miner.calculateNextDifficulty
(
	nAverageDifficulty,
	nTimeUsed,
	nTimeStandard,
	function( err, oData )
	{
		//
		//	oData
		//	{ difficulty : uNextDifficulty }
		//
		if ( err )
		{
			return console.error( err );
		}

		if ( oData &&
			'object' === typeof oData )
		{
			if ( oData.hasOwnProperty( 'difficulty' ) &&
				'number' === typeof oData.difficulty &&
				oData.difficulty > 0 )
			{
				console.log( `new difficulty value after ${ nAverageDifficulty } is : ${ oData.difficulty }` );
			}
			else
			{
				console.log( `calculateNextDifficulty callback :: invalid value .difficulty` );
			}
		}
		else
		{
			console.log( `calculateNextDifficulty callback :: invalid oData object` );
		}
	}
);