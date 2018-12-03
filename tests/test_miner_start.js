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

sInputHeaderHex		= 'f93c6f10ded14bc3d2351389bdf8aa50aed5e1ea2182cc9e031a2c75f05ccec9ac96251ec756ad5dfc75935f05a02f5f48cdf5ad0a090830dbb78cbebb198d664d66fd93d3c04184af48a5ad093a9f79fdb07888875a1d3b0aab54993becd9edc75ef252447cb2da6b99c9df021007cb06463a97382c4776dfa76aa34dbc5c39d67428e4fc9dc99cb736db4b';
bufInputHeader	= Buffer.from( sInputHeaderHex, 'hex' );
uBits			= 528548564;

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
