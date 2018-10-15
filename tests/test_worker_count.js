const _os			= require( 'os' );



const CPU_LIST_ITEM		= _os.cpus()[ 0 ];

let g_arrCpuList		= [];


for ( let i = 0; i < 128; i ++ )
{
	g_arrCpuList.push( CPU_LIST_ITEM );
}

console.log( `############################################################` );
console.log( `### TEST for _getDefaultMaxWorkerCount()` );
console.log( `############################################################` );

for ( let i = 0; i < g_arrCpuList.length; i ++ )
{
	let arrMockCpuList	= g_arrCpuList.slice( 0, i );
	let nMaxWorkerCount	= _getDefaultMaxWorkerCount( arrMockCpuList );

	console.log( `# Total cores of CPU : ${ String( arrMockCpuList.length ).padStart( 3 ) }, default cores for miner : ${ String( nMaxWorkerCount ).padStart( 3 ) }.` );
}




/**
 *	get default value of max worker counter
 *
 *	@return	{number}
 *	@private
 */
function _getDefaultMaxWorkerCount( arrCpuList )
{
	let nRet = 1;

	if ( Array.isArray( arrCpuList ) && arrCpuList.length > 1 )
	{
		if ( arrCpuList.length < 4 )
		{
			//	1 core for main, others for miner
			nRet = arrCpuList.length - 1;
		}
		else if ( arrCpuList.length >= 4 && arrCpuList.length <= 8 )
		{
			//	2 cores for main, others for miner
			nRet = arrCpuList.length - 2;
		}
		else if ( arrCpuList.length > 8 )
		{
			//	4 cores for main, others for miner
			nRet = arrCpuList.length - 4;
		}
	}

	return nRet;
}