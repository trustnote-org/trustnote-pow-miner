process.env.ENV_TRUST_MINER_DEBUG = true;

/**
 *	@modules
 */
const _miner	= require( '../js/index.js' );






/**
 *	start here
 */
let _oOptions	=
	{
		bufInputHeader	: new Buffer( 140 ),
		bits		: _miner.getBitsByTarget( "0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff" ),
		calcTimes	: 30,
		maxLoop		: 1000000,
		maxWorkerCount	: 0,
	};

_miner.startMining( _oOptions, function( err, oData )
{
	if ( null === err )
	{
		if ( oData )
		{
			//
			//	check proof of work
			//
			_miner.checkProofOfWork( _oOptions.bufInputHeader, _oOptions.bits, oData.nonce, oData.hashHex, ( err, oResult ) =>
			{
				console.log( `checkProofOfWork 1, err : ${ err }, oResult : `, oResult );
			});
			_miner.checkProofOfWork( _oOptions.bufInputHeader, _oOptions.bits, 11111, oData.hashHex, ( err, oResult ) =>
			{
				console.log( `checkProofOfWork 2, err : ${ err }, oResult : `, oResult );
			});
			_miner.checkProofOfWork( _oOptions.bufInputHeader, _oOptions.bits, oData.nonce, '111111', ( err, oResult ) =>
			{
				console.log( `checkProofOfWork 3, err : ${ err }, oResult : `, oResult );
			});


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
