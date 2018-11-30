process.env.ENV_TRUST_MINER_DEBUG = true;

/**
 *	@modules
 */
const _miner	= require( '../js/index.js' );






/**
 *	start here
 */
let sInputHeaderHex	= null;
let bufInputHeader	= Buffer.alloc( 140 );
let uBits		= _miner.getBitsByTarget( "00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff" );

sInputHeaderHex		= 'e4b4f0b4727c33287a584acbadc1d112f3553a0efc21524b0a486c9cbbf13c2fd95a7dadc42380dcff7f90f4cb4fc519f6119c0fbe0a690d0fa09dce06920af2c8d32e2d9c98ee23289b22eb90b2b1c644df9bbd648813c0dc01a4d59d8774d14c2cb1a62711698af00fb14ed2f281e4202c389144294a4c00fd67a2f400db8967ea767343d9c9b359d5000a';
bufInputHeader	= Buffer.from( sInputHeaderHex, 'hex' );
uBits			= 537395199;

let _oOptions		=
	{
		bufInputHeader	: bufInputHeader,
		bits		: uBits,
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
