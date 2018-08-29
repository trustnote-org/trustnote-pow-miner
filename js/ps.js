'use strict';

const { spawn }		= require( 'child_process' );
const es		= require( 'event-stream' );




function childrenOfPid( pid, callback )
{
	let headers = null;

	if ( 'function' !== typeof callback )
	{
		throw new Error( 'childrenOfPid(pid, callback) expects callback' );
	}
	if ( 'number' === typeof pid )
	{
		pid = pid.toString();
	}

	//
	//	The `ps-tree` module behaves differently on *nix vs. Windows
	//	by spawning different programs and parsing their output.
	//
	//	Linux:
	//	1. " <defunct> " need to be striped
	//	```bash
	//	$ ps -A -o comm,ppid,pid,stat
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

	let processLister;
	if ( process.platform === 'win32' )
	{
		//	See also: https://github.com/nodejs/node-v0.x-archive/issues/2318
		processLister = spawn( 'wmic.exe', [ 'PROCESS', 'GET', 'Name,ProcessId,ParentProcessId,Status' ] );
	}
	else
	{
		processLister = spawn( 'ps', [ '-A', '-o', 'ppid,pid,stat,comm' ] );
	}

	es.connect
	(
		//	spawn('ps', ['-A', '-o', 'ppid,pid,stat,comm']).stdout,
		processLister.stdout,
		es.split(),
		es.map( ( line, cb ) =>
		{
			//	this could parse alot of unix command output
			let columns	= line.trim().split( /\s+/ );
			if ( ! headers )
			{
				headers = columns;

				//
				//	Rename Win32 header name, to as same as the linux, for compatible.
				//
				headers	= headers.map( normalizeHeader );
				return cb();
			}

			let row = {};
			// For each header
			let h = headers.slice();
			while (h.length) {
				row[h.shift()] = h.length ? columns.shift() : columns.join(' ');
			}

			return cb(null, row);
		}),
		es.writeArray(function (err, ps) {
			let parents = [pid],
				children = [];

			ps.forEach(function (proc) {
				if (parents.indexOf(proc.PPID) !== -1) {
					parents.push(proc.PID)
					children.push(proc)
				}
			});

			callback(null, children);
		})
	).on('error', callback)
}

/**
 * Normalizes the given header `str` from the Windows
 * title to the *nix title.
 *
 * @param {string} str Header string to normalize
 */
function normalizeHeader(str) {
	if (process.platform !== 'win32') {
		return str;
	}

	switch (str) {
		case 'Name':
			return 'COMMAND';
			break;
		case 'ParentProcessId':
			return 'PPID';
			break;
		case 'ProcessId':
			return 'PID';
			break;
		case 'Status':
			return 'STAT';
			break;
		default:
			throw new Error('Unknown process listing header: ' + str);
	}
}




/**
 * 	@exports
 */
module.exports	= childrenOfPid;