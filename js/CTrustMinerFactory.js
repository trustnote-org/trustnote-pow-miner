'use strict';

const { spawn }			= require( 'child_process' );
const _os			= require( 'os' );
const _fs			= require( 'fs' );
const _children			= require( './children.js' );
const _utility			= require( './utility.js' );


/**
 *	@constants
 */
const DEFAULT_CALC_TIMES	= 30;
const DEFAULT_MAX_LOOP		= 100000000;
const CPU_LIST			= _os.cpus();
const DEFAULT_WORKER_COUNT	= Array.isArray( CPU_LIST ) ? CPU_LIST.length - 1 : 1;
const PID_FULL_FILENAME		= `${ _os.tmpdir() }/trustnote-pow-miner.pid`;




/**
 *	@class	CTrustMinerFactory
 */
class CTrustMinerFactory
{
	constructor()
	{
		/**
		 *	initialize workers
		 */
		this.m_arrWorkers	= [];
		this.m_nLoopStart	= 0;
		this.m_bAlreadyWin	= false;
		this.m_arrAllResults	= [];

		this._initWorkers();
	}


	/**
	 * 	@public
	 *	launch all mining workers
	 *
	 *	@param	{object}	oOptions
	 *	@param	{Buffer}	oOptions.bufInputHeader		with length 140 bytes
	 *	@param	{number}	oOptions.difficulty
	 *	@param	{function}	pfnCallback( err, { win : true, hashHex : sActualHashHex, nonce : uActualNonce } )
	 */
	launch( oOptions, pfnCallback )
	{
		if ( null === oOptions || 'object' !== typeof oOptions )
		{
			return pfnCallback( `invalid oOptions, not a plain object.` );
		}
		if ( null === oOptions.bufInputHeader || 'object' !== typeof oOptions.bufInputHeader )
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

		let oOptionsCp		= Object.assign
		(
			{
				calcTimes	: DEFAULT_CALC_TIMES,
				maxLoop		: DEFAULT_MAX_LOOP,
			},
			oOptions
		);
		oOptionsCp.inputHeader	= oOptionsCp.bufInputHeader.toString( 'hex' );

		//
		//	save master pid
		//
		this.saveMasterPId();

		//
		//	stop all workers
		//
		this._stopAllWorkers();

		//
		//	launch all workers
		//
		this._checkWorkers( oOptionsCp, ( err, oResult ) =>
		{
			this.m_arrAllResults.push( { err : err, result : oResult } );
		});

		//	...
		this._waitForWinnerWorkerDone( err =>
		{
			let oResult;
			let i;
			let oItem;

			//	...
			oResult	= null;

			for ( i = 0; i < this.m_arrAllResults.length; i ++ )
			{
				oItem	= this.m_arrAllResults[ i ];
				if ( null === oItem.err &&
					this._isResultOfWin( oItem.result ) )
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
				for ( i = 0; i < this.m_arrAllResults.length; i ++ )
				{
					oItem	= this.m_arrAllResults[ i ];
					if ( null === oItem.err &&
						this._isResultOfGameOver( oItem.result ) )
					{
						oResult	= Object.assign( { win : false }, oItem.result );
						break;
					}
				}
			}

			pfnCallback( null, oResult );
		});
	}

	/**
	 * 	@public
	 *	destroy all workers
	 *
	 * 	@param	{number}	nMasterPId
	 * 	@param	{function}	pfnCallback( err, nKilled )
	 * 	@return	{boolean}
	 */
	destroy( nMasterPId, pfnCallback )
	{
		if ( ! _utility.isNumeric( nMasterPId ) || nMasterPId <= 0 )
		{
			pfnCallback( `invalid parameter nMasterPId` );
			return false;
		}

		_children( nMasterPId, ( err, arrChildren ) =>
		{
			let nKilled	= 0;

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
					if ( oChild &&
						'object' === typeof oChild &&
						oChild.hasOwnProperty( 'PID' ) )
					{
						let nChildPId	= parseInt( oChild[ 'PID' ] );
						nKilled += this._stopWorker( nChildPId ) ? 1 : 0;
					}
				}
			}

			//
			//	will return the number of processes been killed
			//
			pfnCallback( null, nKilled );
		});

		return true;
	}

	/**
	 * 	@public
	 *	get master pid
	 *
	 *	@return {number}
	 */
	getMasterPId()
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
	 *	@public
	 *	save master pid
	 */
	saveMasterPId()
	{
		return _fs.writeFileSync( PID_FULL_FILENAME, process.pid );
	}


	/**
	 * 	@private
	 *	stop/kill worker by pid
	 *
	 *	@param	{number}	nPId
	 *	@return {boolean}
	 */
	_stopWorker( nPId )
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
	 * 	@private
	 *	stop/kill all workers
	 */
	_stopAllWorkers()
	{
		if ( ! Array.isArray( this.m_arrWorkers ) || 0 === this.m_arrWorkers.length )
		{
			return -1;
		}

		//	...
		let nRet = 0;

		for ( let i = 0; i < this.m_arrWorkers.length; i ++ )
		{
			if ( this.m_arrWorkers[ i ].handle &&
				this.m_arrWorkers[ i ].handle.pid )
			{
				nRet += this._stopWorker( this.m_arrWorkers[ i ].handle.pid ) ? 1 : 0;
			}
		}

		return nRet;
	}

	/**
	 * 	@private
	 *	wait for all workers done
	 *
	 *	@param	{function}	pfnCallback( err )
	 */
	_waitForWinnerWorkerDone( pfnCallback )
	{
		//
		//	check for wined workers
		//
		let arrWinResults = this.m_arrAllResults.filter( oResult =>
		{
			return this._isResultOfWin
			(
				( oResult && 'object' === oResult ) ? oResult.result : null
			);
		});
		if ( arrWinResults.length > 0 )
		{
			//	one or more workers already win
			return pfnCallback( null );
		}


		//
		//	check and wait for all workers
		//
		let arrLiveWorkers = this.m_arrWorkers.filter( oWorker =>
		{
			return _utility.isObjectWithKeys( oWorker, [ 'handle' ] ) &&
				_utility.isObjectWithKeys( oWorker.handle, 'pid' ) &&
				this._isWorkerExists( oWorker.handle.pid );
		});

		if ( arrLiveWorkers.length > 0 )
		{
			setTimeout( () =>
			{
				this._waitForWinnerWorkerDone( pfnCallback );

			}, 500 );
		}
		else
		{
			pfnCallback( null );
		}
	}

	/**
	 * 	@private
	 * 	detect if a worker with pid {nPID} exists
	 *
	 *	@param	{number}	nPId
	 *	@return {boolean}
	 */
	_isWorkerExists( nPId )
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
	 * 	@private
	 *	check workers
	 *
	 *	@param	{object}	oOptions
	 *	@param	{string}	oOptions.bufInputHeader		hex string with length of 280 bytes
	 *	@param	{number}	oOptions.calcTimes
	 *	@param	{number}	oOptions.maxLoop
	 *	@param	{number}	oOptions.difficulty
	 *	@param	{function}	pfnCallback( err, { hashHex : sActualHashHex, nonce : uActualNonce } )
	 *				TIME IS OVER
	 *					( null, { gameOver : true, hashHex : null, nonce : 0 } );
	 */
	_checkWorkers( oOptions, pfnCallback )
	{
		if ( ! Array.isArray( this.m_arrWorkers ) || 0 === this.m_arrWorkers.length )
		{
			pfnCallback( `Workers was not initialized` );
			return false;
		}
		if ( this.m_bAlreadyWin )
		{
			pfnCallback( `We already win.` );
			return false;
		}
		if ( this.m_nLoopStart >= oOptions.maxLoop )
		{
			//	`Time is over.`
			pfnCallback( null, { gameOver : true, hashHex : null, nonce : 0 } );
			return false;
		}

		for ( let i = 0; i < this.m_arrWorkers.length; i ++ )
		{
			let nWorkerId	= i;
			let nCalcTimes	= oOptions.calcTimes;
			let nStart	= this.m_nLoopStart * nCalcTimes;

			//	...
			if ( null === this.m_arrWorkers[ i ].handle ||
				! this._isWorkerExists( this.m_arrWorkers[ i ].handle.pid ) )
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
				this.m_arrWorkers[ i ].handle = this._spawnWorker( oOptionsCopy, pfnCallback );
				this.m_nLoopStart ++;
			}
		}
	}

	/**
	 * 	@private
	 *	check win
	 *
	 *	@param sData
	 *	@return {object}
	 *		return an plain object if wins
	 *		{
	 *			hashHex	: 'hash string',
	 *			nonce	: 000,
	 *		}
	 *		otherwise, return null
	 */
	_checkWin( sData )
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
						arrJson[ 1 ] &&
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
	 * 	@private
	 *	spawn worker
	 *
	 *	@param	{object}	oOptions
	 *	@param 	{function}	pfnCallback( err, { hashHex : string, nonce : number } )
	 *	@return	{*}
	 */
	_spawnWorker( oOptions, pfnCallback )
	{
		let oOptionsCp;
		let arrArgs;
		let hHandle;

		oOptionsCp	= Object.assign( {}, oOptions );
		delete oOptionsCp.bufInputHeader;
		delete oOptionsCp.maxLoop;

		console.log( `>|< trustnote-pow-miner spawnWorker by master(${ process.pid }) with options : `, oOptionsCp );

		//	...
		arrArgs	= [ `${ __dirname }/worker.js`, process.pid, JSON.stringify( oOptionsCp ) ];
		hHandle	= spawn( 'node', arrArgs );
		if ( hHandle )
		{
			hHandle.stdout.on( 'data', ( sData ) =>
			{
				//console.log( `child stdout:\n${ sData }` );
				let jsonResult = this._checkWin( sData );
				if ( jsonResult )
				{
					//
					//	WIN !!!
					//
					this.m_bAlreadyWin = true;
					this._stopAllWorkers();

					//	...
					pfnCallback( null, jsonResult );
				}
			});
			hHandle.on( 'disconnect', () =>
			{
				//console.log(`child disconnect:\n`);
			});
			hHandle.on( 'exit', ( nCode, sSignal ) =>
			{
				//console.log( `child process exited with code ${ nCode } and signal ${ sSignal }` );
				this._checkWorkers( oOptions, pfnCallback );
			});
		}

		return hHandle;
	}

	/**
	 * 	@private
	 * 	check if the item is a valid result
	 *
	 *	@param	{object}	oItem
	 *	@return {boolean}
	 */
	_isResultOfWin( oItem )
	{
		return _utility.isObjectWithKeys( oItem, [ 'hashHex', 'nonce' ] )  &&
			_utility.isString( oItem.hashHex ) &&
			64 === oItem.hashHex.length &&
			_utility.isNumeric( oItem.nonce ) &&
			oItem.nonce > 0;
	}

	/**
	 * 	@private
	 *	check if the game is over
	 *
	 *	@param	{object}	oItem
	 *	@return {boolean}
	 */
	_isResultOfGameOver( oItem )
	{
		return _utility.isObjectWithKeys( oItem, 'gameOver' ) &&
			_utility.isBool( oItem.gameOver ) &&
			oItem.gameOver;
	}

	/**
	 * 	@private
	 *	initialize workers
	 */
	_initWorkers()
	{
		this.m_arrWorkers	= [];
		this.m_nLoopStart	= 0;
		this.m_bAlreadyWin	= false;
		this.m_arrAllResults	= [];

		for ( let i = 0; i < DEFAULT_WORKER_COUNT; i ++ )
		{
			this.m_arrWorkers[ i ] = Object.assign( {}, CPU_LIST[ i ] );
			this.m_arrWorkers[ i ][ 'workerId' ]	= i;
			this.m_arrWorkers[ i ][ 'handle' ]	= null;
		}
	}
}




/**
 * 	@exports
 */
module.exports	= CTrustMinerFactory;
