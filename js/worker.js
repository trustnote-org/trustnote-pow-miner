const CTrustMinerLibrary	= require( './CTrustMinerLibrary.js' );

/**
 * 	@variables
 */
let _oLibrary		= new CTrustMinerLibrary();

/**
 *	command arguments
 *	@type {Array}
 */
let _arrArgv		= process.argv;
let _nMasterPId		= ( Array.isArray( _arrArgv ) && _arrArgv.length >= 3 ) ? parseInt( _arrArgv[ 2 ] ) : null;
let _nWorkerId		= ( Array.isArray( _arrArgv ) && _arrArgv.length >= 4 ) ? parseInt( _arrArgv[ 3 ] ) : null;
let _nNonceStart	= ( Array.isArray( _arrArgv ) && _arrArgv.length >= 5 ) ? parseInt( _arrArgv[ 4 ] ) : null;
let _nCalcTimes		= ( Array.isArray( _arrArgv ) && _arrArgv.length >= 6 ) ? parseInt( _arrArgv[ 5 ] ) : null;


function isMasterExists()
{
	let bRet = false;

	try
	{
		bRet = process.kill( _nMasterPId, '0' );
	}
	catch ( e )
	{
	}

	return bRet;
}

function exitWorker( nCode )
{
	setTimeout( () =>
	{
		process.exit( nCode );

	}, 3000 );
}

console.log( `_nMasterPId : ${ _nMasterPId },
		_nWorkerId : ${ _nWorkerId },
		_nNonceStart : ${ _nNonceStart },
		_nWorkerId : ${ _nNonceStart },
		_nCalcTimes : ${ _nCalcTimes }` );

while( _nCalcTimes > 0 )
{
	/**
	 * 	master had gone
	 */
	if ( ! isMasterExists() )
	{
		exitWorker( 1000 );
	}


	/**
	 *	...
	 */
	let nNonce	= _nNonceStart - _nCalcTimes;
	//console.log( `### CHILD[ ${ _nWorkerId } ], nonce : ${ nNonce }.` );
	if ( 1000001 === nNonce )
	{
		console.log( JSON.stringify( [ 0, { result : 'win', nonce : nNonce } ] ) );
		exitWorker( 2000 );
		break;
	}

	/**
	 * 	go on
	 */
	_nCalcTimes --;
}


// setInterval( () =>
// {
// 	console.log( `@@@ TIMER ${ Date.now() }` );
// }, 1000 );

