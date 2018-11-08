process.env.ENV_TRUST_MINER_DEBUG = true;

/**
 *	@modules
 */
const _miner	= require( '../js/index.js' );






/**
 *	start here
 */
let _arrUBitsList	= [
	453025859,
	_miner.getBitsByTarget( "00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff" ),
];
for ( let i = 0; i < _arrUBitsList.length; i ++ )
{
	let uBits	= _arrUBitsList[ i ];
	_miner.getTargetByBits( uBits, function( err, sTarget )
	{
		console.log( `_miner.getTargetByBits( ${ uBits } ), err: ${ err }, sTarget: ${ sTarget } ` );
	});
}