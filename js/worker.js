'use strict';

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
let _sArgString		= ( Array.isArray( _arrArgv ) && _arrArgv.length >= 4 ) ? _arrArgv[ 3 ] : '';
let _jsonArg		= null;





function isMasterExists()
{
	let bRet = false;

	try
	{
		bRet = process.kill( _nMasterPId, 0 );
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

	}, 100 );
}

function parseArgs()
{
	_jsonArg = JSON.parse( _sArgString );
	if ( 'object' !== typeof _jsonArg )
	{
		throw new Error( `invalid format of arguments.` );
	}
	if ( ! _jsonArg.hasOwnProperty( 'workerId' ) )
	{
		throw new Error( `invalid arguments.workerId.` );
	}
	if ( ! _jsonArg.hasOwnProperty( 'start' ) )
	{
		throw new Error( `invalid arguments.start.` );
	}
	if ( ! _jsonArg.hasOwnProperty( 'calcTimes' ) )
	{
		throw new Error( `invalid arguments.calcTimes.` );
	}
	if ( ! _jsonArg.hasOwnProperty( 'inputHeader' ) )
	{
		throw new Error( `invalid arguments.inputHeader.` );
	}
	if ( ! _jsonArg.hasOwnProperty( 'difficulty' ) )
	{
		throw new Error( `invalid arguments.difficulty.` );
	}
}

function startWorker()
{
	parseArgs();

	console.log( _jsonArg );

	//	convert inputHeader from hex to Buffer
	_jsonArg.inputHeader	= Buffer.from( _jsonArg.inputHeader, 'hex' );
	_jsonArg.start		= parseInt( _jsonArg.start );
	_jsonArg.calcTimes	= parseInt( _jsonArg.calcTimes );
	_jsonArg.difficulty	= parseInt( _jsonArg.difficulty );


	/**
	 * 	master had gone
	 */
	if ( ! isMasterExists() )
	{
		exitWorker( 1000 );
	}


	//	...
	_oLibrary.startMining
	(
		_jsonArg.inputHeader,
		_jsonArg.difficulty,
		_jsonArg.start,
		_jsonArg.calcTimes,
		function( err, oData )
		{
			if ( null == err )
			{
				console.log( JSON.stringify( [ 0, oData ] ) );
			}

			//	...
			exitWorker( 2000 );
		}
	);
}




/**
 *	start worker
 */
startWorker();
