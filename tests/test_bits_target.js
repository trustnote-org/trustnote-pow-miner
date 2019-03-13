process.env.ENV_TRUST_MINER_DEBUG = true;

/**
 *	@modules
 */
const _miner	= require( '../js/index.js' );






/**
 *	start here
 */
let _arrUBitsList	= [
	92179269,
	22216704,
	453025859,
	528482303,
	536905195,
	528979787,
	528548564,
	_miner.getBitsByTarget( "007fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff" ),
];
for ( let i = 0; i < _arrUBitsList.length; i ++ )
{
	let uBits	= _arrUBitsList[ i ];
	_miner.getTargetByBits( uBits, function( err, sTarget )
	{
		console.log( `_miner.getTargetByBits( ${ uBits } ), err: ${ err }, sTarget: ${ sTarget } ` );
	});
}
