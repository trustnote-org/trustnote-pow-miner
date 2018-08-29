'use strict';

const { spawn }		= require( 'child_process' );
const _event_stream	= require( 'event-stream' );




/**
 *	enumerateChildrenByPId
 *	@param	{number}	nPId
 *	@param	{function}	pfnCallback( err, arrChildrenList )
 */
function enumerateChildrenByPId( nPId, pfnCallback )
{
	let arrHeaders = null;

	if ( 'function' !== typeof pfnCallback )
	{
		throw new Error( `call enumerateChildrenByPId with invalid pfnCallback.` );
	}
	if ( 'number' === typeof nPId )
	{
		nPId = nPId.toString();
	}

	//
	//	The `children` module behaves differently on *nix vs. Windows
	//	by spawning different programs and parsing their output.
	//
	//	Linux:
	//	1. " <defunct> " need to be striped
	//	```bash
	//	$ ps -A -o comm,ppid,nPId,stat
	//	COMMAND          PPID   PID STAT
	//	bbsd             2899 16958 Ss
	//	watch <defunct>  1914 16964 Z
	//	ps              20688 16965 R+
	//	```
	//
	//	Win32:
	//	1. wmic PROCESS WHERE ParentProcessId=4604 GET Name,ParentProcessId,ProcessId,Status)
	//	2. The order of head columns is fixed
	//	```shell
	//	> wmic PROCESS GET Name,ProcessId,ParentProcessId,Status
	//	Name                          ParentProcessId  ProcessId   Status
	//	System Idle Process           0                0
	//	System                        0                4
	//	smss.exe                      4                228
	//	```

	let hProcessLister;
	if ( process.platform === 'win32' )
	{
		//	See also: https://github.com/nodejs/node-v0.x-archive/issues/2318
		hProcessLister = spawn( 'wmic.exe', [ 'PROCESS', 'GET', 'Name,ProcessId,ParentProcessId,Status' ] );
	}
	else
	{
		hProcessLister = spawn( 'ps', [ '-A', '-o', 'ppid,pid,stat,comm' ] );
	}

	_event_stream.connect
	(
		//	spawn('ps', ['-A', '-o', 'ppid,nPId,stat,comm']).stdout,
		hProcessLister.stdout,
		_event_stream.split(),
		_event_stream.map( ( line, cb ) =>
		{
			//	this could parse alot of unix command output
			let arrColumns	= line.trim().split( /\s+/ );
			if ( ! arrHeaders )
			{
				arrHeaders = arrColumns;

				//
				//	Rename Win32 header name, to as same as the linux, for compatible.
				//
				arrHeaders	= arrHeaders.map( normalizeHeader );
				return cb();
			}

			//	For each header
			let oRow = {};
			let arrHeadersCopy = arrHeaders.slice();
			while ( arrHeadersCopy.length )
			{
				oRow[ arrHeadersCopy.shift() ] = arrHeadersCopy.length ? arrColumns.shift() : arrColumns.join(' ');
			}

			return cb( null, oRow );
		}),
		_event_stream.writeArray( ( err, arrPs ) =>
		{
			let arrParents	= [ nPId ];
			let arrChildren	= [];

			arrPs.forEach( oProcessItem =>
			{
				if ( -1 !== arrParents.indexOf( oProcessItem.PPID ) )
				{
					arrParents.push( oProcessItem.PID );
					arrChildren.push( oProcessItem );
				}
			});

			pfnCallback( null, arrChildren );
		})

	).on( 'error', pfnCallback );
}


/**
 *	Normalizes the given header `sString` from the Windows title to the *nix title.
 *
 *	@param {string}	sString	- Header string to normalize
 */
function normalizeHeader( sString )
{
	if ( 'win32' !== process.platform )
	{
		return sString;
	}

	switch( sString )
	{
		case 'Name':
			return 'COMMAND';
		case 'ParentProcessId':
			return 'PPID';
		case 'ProcessId':
			return 'PID';
		case 'Status':
			return 'STAT';
		default:
			throw new Error( `Unknown process listing header : ${ sString }.` );
	}
}




/**
 * 	@exports
 */
module.exports	= enumerateChildrenByPId;