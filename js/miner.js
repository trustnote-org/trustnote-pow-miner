const _os		= require( 'os' );
const _fs		= require( 'fs' );
const { spawn }		= require( 'child_process' );


const CALCULATE_TIMES	= 10000;

let _arrCPUs		= _os.cpus();
let _nWorkerCount	= Array.isArray( _arrCPUs ) ? _arrCPUs.length - 1 : 1;
let _arrWorkers		= null;
let _nLoopStart		= 0;
let _nMaxLoop		= 10000;
let _bAlreadyWin	= false;

let _cWriteStream	= _fs.createWriteStream( 'log.txt', { flags : "a" } );





function initWorker()
{
	_arrWorkers = [];
	for ( let i = 0; i < _nWorkerCount; i ++ )
	{
		_arrWorkers[ i ] = Object.assign( {}, _arrCPUs[ i ] );
		_arrWorkers[ i ][ 'workerId' ]	= i;
		_arrWorkers[ i ][ 'handle' ]	= null;
	}
}

function stopWorker( nPId )
{
	let bRet = false;

	try
	{
		bRet = process.kill( nPId, 'SIGKILL' );
	}
	catch ( e )
	{
	}

	return bRet;
}
function stopAllWorkers()
{
	for ( let i = 0; i < _nWorkerCount; i ++ )
	{
		if ( _arrWorkers[ i ].handle &&
			_arrWorkers[ i ].handle.pid )
		{
			stopWorker( _arrWorkers[ i ].handle.pid );
		}
	}
}

function isWorkerExists( nPId )
{
	let bRet = false;

	try
	{
		bRet = process.kill( nPId, 0 );
	}
	catch ( e )
	{
	}

	return bRet;
}


/**
 *	check workers
 *
 *	@param	{Buffer}	bufInputHeader
 *	@param	{number}	uDifficulty
 *	@param	{function}	pfnCallback( err, { hashHex : sActualHashHex, nonce : uActualNonce } )
 */
function checkWorkers( bufInputHeader, uDifficulty, pfnCallback )
{
	if ( ! Array.isArray( _arrWorkers ) || 0 === _arrWorkers.length )
	{
		pfnCallback( `Workers was not initialized` );
		return false;
	}
	if ( _bAlreadyWin )
	{
		pfnCallback( `We already win.` );
		return false;
	}
	if ( _nLoopStart >= _nMaxLoop )
	{
		//	`Time is over.`
		pfnCallback( null, { error : 'timeout', hashHex : null, nonce : 0 } );
		return false;
	}

	for ( let i = 0; i < _arrWorkers.length; i ++ )
	{
		let nWorkerId	= i;
		let nCalcTimes	= CALCULATE_TIMES;
		let nStart	= _nLoopStart * nCalcTimes;

		//	...
		if ( null === _arrWorkers[ i ].handle ||
			! isWorkerExists( _arrWorkers[ i ].handle.pid ) )
		{
			let jsonArg = {
				workerId	: nWorkerId,
				start		: nStart,
				calcTimes	: nCalcTimes,
				inputHeader	: bufInputHeader,
				difficulty	: uDifficulty,
			};
			_arrWorkers[ i ].handle = spawnWorker( JSON.stringify( jsonArg ), bufInputHeader, uDifficulty, pfnCallback );
			_nLoopStart ++;
		}
	}
}


/**
 *	check win
 *
 *	@param sData
 *	@return {boolean}
 */
function checkWin( sData )
{
	let oRet = null;

	try
	{
		sData		= String.fromCharCode.apply( null, sData );
		let arrData	= sData.split( "\n" );
		if ( Array.isArray( arrData ) )
		{
			for ( let i = 0; i < arrData.length; i ++ )
			{
				let arrJson	= JSON.parse( arrData[ i ] );
				if ( Array.isArray( arrJson ) &&
					2 === arrJson.length &&
					0 === arrJson[ 0 ] &&
					'object' === typeof( arrJson[ 1 ] ) &&
					arrJson[ 1 ].hasOwnProperty( 'hashHex' ) &&
					arrJson[ 1 ].hasOwnProperty( 'nonce' ) )
				{
					oRet = arrJson[ 1 ];
					break;
				}
			}
		}
	}
	catch ( e )
	{
	}

	return oRet;
}

/**
 *	spawn worker
 *
 *	@param	{string}	sArgString
 *	@param 	{function}	pfnCallback
 *	@return	{*}
 */
function spawnWorker( sArgString, bufInputHeader, uDifficulty, pfnCallback )
{
	let hHandle = spawn
	(
		'node',
		[
			'worker.js',
			process.pid,
			sArgString
		]
	);
	if ( hHandle )
	{
		hHandle.stdout.on( 'data', ( sData ) =>
		{
			//_cWriteStream.write( sData );
			console.log( `child stdout:\n${ sData }` );

			let jsonResult = checkWin( sData );
			if ( jsonResult )
			{
				_bAlreadyWin = true;
				console.log( `WINNER WINNER CHICKEN DINNER!` );
				stopAllWorkers();

				//	...
				pfnCallback( null, jsonResult );
			}
		});
		hHandle.on( 'disconnect', () =>
		{
			//console.log(`child disconnect:\n`);
		});
		hHandle.on( 'exit', function( nCode, sSignal )
		{
			//console.log( `child process exited with code ${ nCode } and signal ${ sSignal }` );
			checkWorkers( bufInputHeader, uDifficulty, pfnCallback );
		});
		// hHandle.on( 'close', function( nCode, sSignal )
		// {
		// 	console.log( `child process closed with code ${ nCode } and signal ${ sSignal }` );
		// 	checkWorkers();
		// });
	}

	return hHandle;
}


/**
 *	start mining
 *
 *	@param	{Buffer}	bufInputHeader
 *	@param	{number}	uDifficulty
 *	@param	{function}	pfnCallback( err, { hashHex : sActualHashHex, nonce : uActualNonce } )
 */
function start( bufInputHeader, uDifficulty, pfnCallback )
{
	initWorker();
	checkWorkers( bufInputHeader, uDifficulty, pfnCallback );
}




/**
 *	start here
 */
let _bufInput		= new Buffer( 140 );
let _uDifficulty	= 536936447;

start( _bufInput, _uDifficulty, function( err, oData )
{
	console.log( err, oData );
});





/**
 * 	@exports
 */
module.exports.start	= start;