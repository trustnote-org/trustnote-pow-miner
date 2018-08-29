'use strict';

const { spawn }			= require( 'child_process' );
const _os			= require( 'os' );
const _fs			= require( 'fs' );
const _children			= require( './children.js' );



/**
 *	@constants
 */
const CALC_TIMES_PER_LOOP	= 30;
const MAX_LOOP			= 10000000;
const CPU_LIST			= _os.cpus();
const MAX_WORKER_COUNT		= Array.isArray( CPU_LIST ) ? CPU_LIST.length - 1 : 1;
const PID_FULL_FILENAME		= `${ _os.tmpdir() }/trustnote-pow-miner.pid`;


/**
 *	@variables
 */
let _arrWorkers			= null;
let _nLoopStart			= 0;
let _bAlreadyWin		= false;
let _arrAllResults		= [];



/**
 *	initialize workers
 */
function initWorkers()
{
	_arrAllResults	= [];
	_arrWorkers	= [];
	for ( let i = 0; i < MAX_WORKER_COUNT; i ++ )
	{
		_arrWorkers[ i ] = Object.assign( {}, CPU_LIST[ i ] );
		_arrWorkers[ i ][ 'workerId' ]	= i;
		_arrWorkers[ i ][ 'handle' ]	= null;
	}
}

/**
 *	stop/kill worker by pid
 *
 *	@param	{number}	nPId
 *	@return {boolean}
 */
function stopWorker( nPId )
{
	let bRet = false;

	try
	{
		console.log( `will kill process ${ nPId } SIGKILL` );
		bRet = process.kill( nPId, 'SIGKILL' );
	}
	catch ( e )
	{
	}

	return bRet;
}

/**
 *	stop/kill all workers
 */
function stopAllWorkers()
{
	for ( let i = 0; i < MAX_WORKER_COUNT; i ++ )
	{
		if ( _arrWorkers[ i ].handle &&
			_arrWorkers[ i ].handle.pid )
		{
			stopWorker( _arrWorkers[ i ].handle.pid );
		}
	}
}

/**
 *	wait for all workers done
 *
 *	@param	{function}	pfnCallback( err )
 */
function waitForWinnerWorkerDone( pfnCallback )
{
	//
	//	check for wined workers
	//
	let arrWinResults = _arrAllResults.filter( oResult =>
	{
		return isResultOfWin( oResult.result );
	});
	if ( arrWinResults.length > 0 )
	{
		//	one or more workers already win
		return pfnCallback( null );
	}


	//
	//	check and wait for all workers
	//
	let arrLiveWorkers = _arrWorkers.filter( oWorker =>
	{
		return 'object' === typeof oWorker &&
			oWorker.handle &&
			oWorker.handle.hasOwnProperty( 'pid' ) &&
			oWorker.handle.pid &&
			isWorkerExists( oWorker.handle.pid );
	});

	if ( arrLiveWorkers.length > 0 )
	{
		setTimeout( () =>
		{
			waitForWinnerWorkerDone( pfnCallback );
		}, 500 );
	}
	else
	{
		pfnCallback( null );
	}
}

/**
 * 	detect if a worker with pid {nPID} exists
 *
 *	@param	{number}	nPId
 *	@return {boolean}
 */
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
 *	@param	{object}	oOptions
 *	@param	{string}	oOptions.bufInputHeader		hex string with length of 280 bytes
 *	@param	{number}	oOptions.calcTimes
 *	@param	{number}	oOptions.maxLoop
 *	@param	{number}	oOptions.difficulty
 *	@param	{function}	pfnCallback( err, { hashHex : sActualHashHex, nonce : uActualNonce } )
 */
function checkWorkers( oOptions, pfnCallback )
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
	if ( _nLoopStart >= oOptions.maxLoop )
	{
		//	`Time is over.`
		pfnCallback( null, { gameOver : true, hashHex : null, nonce : 0 } );
		return false;
	}

	for ( let i = 0; i < _arrWorkers.length; i ++ )
	{
		let nWorkerId	= i;
		let nCalcTimes	= oOptions.calcTimes;
		let nStart	= _nLoopStart * nCalcTimes;

		//	...
		if ( null === _arrWorkers[ i ].handle ||
			! isWorkerExists( _arrWorkers[ i ].handle.pid ) )
		{
			let oOptionsCopy	= Object.assign
				(
					{},
					oOptions,
					{
						workerId	: nWorkerId,
						start		: nStart,
					}
				);
			_arrWorkers[ i ].handle = spawnWorker( oOptionsCopy, pfnCallback );
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
 *	@param	{object}	oOptions
 *	@param 	{function}	pfnCallback
 *	@return	{*}
 */
function spawnWorker( oOptions, pfnCallback )
{
	let oOptionsCp;
	let arrArgs;
	let hHandle;

	oOptionsCp	= Object.assign( {}, oOptions );
	delete oOptionsCp.bufInputHeader;
	delete oOptionsCp.maxLoop;

	arrArgs	= [ `${ __dirname }/worker.js`, process.pid, JSON.stringify( oOptionsCp ) ];
	hHandle	= spawn( 'node', arrArgs );
	if ( hHandle )
	{
		hHandle.stdout.on( 'data', ( sData ) =>
		{
			//console.log( `child stdout:\n${ sData }` );
			let jsonResult = checkWin( sData );
			if ( jsonResult )
			{
				//
				//	WIN !!!
				//
				_bAlreadyWin = true;
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
			checkWorkers( oOptions, pfnCallback );
		});
	}

	return hHandle;
}

/**
 * 	check if the item is a valid result
 *
 *	@param	{object}	oItem
 *	@return {boolean}
 */
function isResultOfWin( oItem )
{
	return 'object' === typeof oItem &&
		oItem.hasOwnProperty( 'hashHex' ) &&
		oItem.hasOwnProperty( 'nonce' ) &&
		'string' === typeof oItem.hashHex &&
		64 === oItem.hashHex.length &&
		'number' === typeof oItem.nonce &&
		oItem.nonce > 0;
}

/**
 *	check if the game is over
 *
 *	@param	{object}	oItem
 *	@return {boolean}
 */
function isResultOfGameOver( oItem )
{
	return 'object' === typeof oItem &&
		oItem.hasOwnProperty( 'gameOver' ) &&
		'boolean' === typeof oItem.gameOver &&
		oItem.gameOver;
}

/**
 *	get master pid
 *	@return {number}
 */
function getMasterPId()
{
	if ( ! _fs.existsSync( PID_FULL_FILENAME ) )
	{
		return 0;
	}

	let nRet	= 0;

	try
	{
		nRet = parseInt( _fs.readFileSync( PID_FULL_FILENAME ) );
	}
	catch( e )
	{
	}

	return nRet;
}

/**
 *	save master pid
 */
function saveMasterPId()
{
	return _fs.writeFileSync( PID_FULL_FILENAME, process.pid );
}


/**
 *	start mining
 *
 *	@param	{object}	oOptions
 *	@param	{Buffer}	oOptions.bufInputHeader		with length 140 bytes
 *	@param	{number}	oOptions.difficulty
 *	@param	{function}	pfnCallback( err, { win : true, hashHex : sActualHashHex, nonce : uActualNonce } )
 */
function start( oOptions, pfnCallback )
{
	if ( 'object' !== typeof oOptions )
	{
		return pfnCallback( `invalid oOptions, not a plain object.` );
	}
	if ( 'object' !== typeof oOptions.bufInputHeader )
	{
		return pfnCallback( `invalid oOptions.bufInputHeader, not a Buffer object.` );
	}
	if ( 140 !== oOptions.bufInputHeader.length )
	{
		return pfnCallback( `invalid oOptions.bufInputHeader, must be a Buffer with length of 140 bytes.` );
	}
	if ( 'number' !== typeof oOptions.difficulty )
	{
		return pfnCallback( `invalid oOptions.difficulty, must be a number.` );
	}

	let bAlreadyCalledBack	= false;
	let oOptionsCp		= Object.assign
		(
			{
				calcTimes	: CALC_TIMES_PER_LOOP,
				maxLoop		: MAX_LOOP,
			},
			oOptions
		);
	oOptionsCp.inputHeader	= oOptionsCp.bufInputHeader.toString( 'hex' );

	//
	//	save master pid
	//
	saveMasterPId();

	//
	//	...
	//
	initWorkers();
	checkWorkers( oOptionsCp, ( err, oResult ) =>
	{
		_arrAllResults.push( { err : err, result : oResult } );
	});

	//	...
	if ( ! bAlreadyCalledBack )
	{
		waitForWinnerWorkerDone( err =>
		{
			let oResult;
			let i;
			let oItem;

			//	...
			oResult	= null;

			for ( i = 0; i < _arrAllResults.length; i ++ )
			{
				oItem	= _arrAllResults[ i ];
				if ( null === oItem.err &&
					isResultOfWin( oItem.result ) )
				{
					oResult	= Object.assign( { win : true }, oItem.result );
					break;
				}
			}
			if ( null === oResult )
			{
				//
				//	detect if the game is over?
				//
				//	{ gameOver : true, hashHex : null, nonce : 0 }
				//
				for ( i = 0; i < _arrAllResults.length; i ++ )
				{
					oItem	= _arrAllResults[ i ];
					if ( null === oItem.err &&
						isResultOfGameOver( oItem.result ) )
					{
						oResult	= Object.assign( { win : false }, oItem.result );
						break;
					}
				}
			}

			pfnCallback( null, oResult );
		});
	}
}


/**
 *	stop all workers
 */
function stop()
{
	let bRet	= false;
	let nMasterPId	= getMasterPId();

	if ( nMasterPId > 0 )
	{
		_children( nMasterPId, function( err, arrChildren )
		{
			//
			//	arrChildren :
			//	[
			// 		{ PPID: '11904', PID: '23336', STAT: 'Rl+', COMMAND: 'node' },
			//		{ PPID: '11904', PID: '23344', STAT: 'Rl+', COMMAND: 'node' },
			//		{ PPID: '11904', PID: '23350', STAT: 'Rl+', COMMAND: 'node' },
			//		{ PPID: '11904', PID: '23357', STAT: 'Rl+', COMMAND: 'node' },
			//	]
			//
			if ( null === err &&
				Array.isArray( arrChildren ) && arrChildren.length > 0 )
			{
				for ( let i = 0; i < arrChildren.length; i ++ )
				{
					let oChild	= arrChildren[ i ];
					if ( 'object' === typeof oChild &&
						oChild.hasOwnProperty( 'PID' ) )
					{
						let nChildPId	= parseInt( oChild[ 'PID' ] );
						stopWorker( nChildPId );
					}
				}
			}

			//
			//	kill master process
			//
			bRet = stopWorker( nMasterPId );
		});
	}

	return bRet;
}





/**
 * 	@exports
 */
module.exports.start	= start;
module.exports.stop	= stop;
