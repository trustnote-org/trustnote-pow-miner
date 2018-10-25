process.env.ENV_TRUST_MINER_DEBUG = true;


const _miner	= require( '../js/index.js' );


let nDifficulty = _miner.getBitsByTarget( "00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff" );
console.log( `nDifficulty value : ${ nDifficulty }` );


const _oDifficultyList	= {};

for ( let i = 0; i <= 64; i ++ )
{
	let sHexString	= getDifficultyHexByPrefixZero( i );
	_oDifficultyList[ sHexString ]	= _miner.getBitsByTarget( sHexString );
}
console.log( _oDifficultyList );




function getDifficultyHexByPrefixZero( nCount )
{
	let sRet	= '';

	//	...
	nCount	= Math.min( nCount, 64 );
	let nC2	= 64 - nCount;

	for ( let i = 0; i < nCount; i ++ )
	{
		sRet += '0';
	}
	for ( let i = 0; i < nC2; i ++ )
	{
		sRet += 'f';
	}

	return sRet;
}



