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

sInputHeaderHex		= '53361928fa83a18646fcee23e66750b336d61129d5e71352b8032c8acaf67da3e793c4cb782712d9a13162e10e7080d966ff785abf708d15f5b01848f90c74764e6b65dae2ec03bcab28531b56786d2bd102ae608f8216d7ad3246111f67960e00a23680e668ba7701e0d0e0ce3a91ac907b1695bb154838c477623c0468b364c4c68ac18cddc846e53207e1';
bufInputHeader	= Buffer.from( sInputHeaderHex, 'hex' );
uBits			= 528979787;

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
