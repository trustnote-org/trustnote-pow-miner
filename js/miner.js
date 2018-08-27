const _os		= require( 'os' );
const { spawn }		= require( 'child_process' );

const CALCULATE_TIMES	= 10000;

let _arrCPUs		= _os.cpus();
let _nWorkerCount	= Array.isArray( _arrCPUs ) ? _arrCPUs.length - 1 : 1;
let _arrWorkers		= null;
let _nLoop		= 0;
let _bAlreadyWin	= false;





function start()
{
	initWorker();
	checkWorkers();
}

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
		bRet = process.kill( nPId, '0' );
	}
	catch ( e )
	{
	}

	return bRet;
}

function checkWorkers()
{
	if ( ! Array.isArray( _arrWorkers ) || 0 === _arrWorkers.length )
	{
		console.log( `Workers was not initialized` );
		return false;
	}
	if ( _bAlreadyWin )
	{
		console.log( `We already win.` );
		return false;
	}


	for ( let i = 0; i < _arrWorkers.length; i ++ )
	{
		let nWorkerId	= i;
		let nCalcTimes	= CALCULATE_TIMES;
		let nStart	= _nLoop * nCalcTimes;

		//	...
		if ( null === _arrWorkers[ i ].handle ||
			! isWorkerExists( _arrWorkers[ i ].handle.pid ) )
		{
			_arrWorkers[ i ].handle = spawnWorker( nWorkerId, nStart, nCalcTimes );
			_nLoop ++;
		}
	}
}

function isWin( sData )
{
	let bRet = false;

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
					arrJson[ 1 ].hasOwnProperty( 'result' ) &&
					'string' === typeof( arrJson[ 1 ][ 'result' ] ) &&
					'win' === arrJson[ 1 ][ 'result' ] )
				{
					bRet = true;
					break;
				}
			}
		}
	}
	catch ( e )
	{
	}

	return bRet;
}

function spawnWorker( nWorkerId, nStart, nTimes )
{
	let hHandle = spawn( 'node', [ 'worker.js', process.pid, nWorkerId, nStart, nTimes ] );
	if ( hHandle )
	{
		hHandle.stdout.on( 'data', ( sData ) =>
		{
			console.log( `child stdout:\n${ sData }` );
			if ( isWin( sData ) )
			{
				_bAlreadyWin = true;
				console.log( `WINNER, WINNER!` );
				stopAllWorkers();
			}
		});
		hHandle.on( 'disconnect', () =>
		{
			console.log(`child disconnect:\n`);
		});
		hHandle.on( 'exit', function( nCode, sSignal )
		{
			console.log( `child process exited with code ${ nCode } and signal ${ sSignal }` );
			checkWorkers();
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
 *	start here
 */
start();




//
// setTimeout( () =>
// {
// 	child.kill( 'SIGINT' );
// 	//child.disconnect();
//
// }, 3000 );
