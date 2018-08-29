const _miner	= require( '../js/index.js' );




/**
 *	start here
 */
let _oOptions	=
	{
		bufInputHeader	: new Buffer( 140 ),
		difficulty	: _miner.difficulty256HexToUInt32( "000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff" ),
		calcTimes	: 30,
		maxLoop		: 1000000,
	};

_miner.startMining( _oOptions, function( err, oData )
{
	if ( null === err )
	{
		if ( oData )
		{
			if ( oData.win )
			{
				console.log( `WINNER WINNER, CHICKEN DINNER!`, oData );
			}
			else if ( oData.gameOver )
			{
				console.log( `GAME OVER!` );
			}
		}
		else
		{
			console.log( `INVALID DATA!` );
		}
	}
	else
	{
		console.log( `OCCURRED ERROR : `, err );
	}
});
