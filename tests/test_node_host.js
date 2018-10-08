const _fs			= require( 'fs' );


/**
 *	search executable host path of node
 *
 *	@return {string}
 *	@private
 */
function _getNodeHostPath()
{
	let sRet = 'node';

	if ( process && 'object' === typeof process &&
		process.argv && 'object' === typeof process.argv &&
		Array.isArray( process.argv ) && process.argv.length > 0 &&
		'string' === typeof process.argv[ 0 ] && process.argv[ 0 ].length > 0 &&
		_fs.existsSync( process.argv[ 0 ] ) )
	{
		sRet = process.argv[ 0 ];
	}

	return sRet;
}



console.log( `node host path : ${ _getNodeHostPath() }` );
