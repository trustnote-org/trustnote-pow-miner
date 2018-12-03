process.env.ENV_TRUST_MINER_DEBUG = false;


const _miner	= require( '../js/index.js' );


let uBits = _miner.getBitsByTarget( "007fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff" );


//
//	calculate next bits
//
let nAverageBits		= uBits;
let nTimeUsed			= 3000;
let nTimeStandard		= 2400;
let dblDeposit			= 0;
let uBombExplodingRoundIndex	= 50;

let sHeader	= ``
	+ "Round".padEnd( 10 )
	+ `|` + "prevBits".padStart( 10 )
	+ `|` + "TimeUsed".padStart( 10 )
	+ `|` + "TimeStd".padStart( 10 )
	+ `|` + "newBits".padStart( 10 )
	+ `|` + "shiftByDeposit".padStart( 16 )
	+ `|` + "shiftByRound".padStart( 16 )
;
console.log( `\n\nBomb will start exploding at round ${ uBombExplodingRoundIndex }.\n`);
console.log( `------------------------------------------------------------------------------------------` );
console.log( sHeader );
console.log( `------------------------------------------------------------------------------------------` );


for ( let uRoundIndex = 1; uRoundIndex < 300; uRoundIndex ++ )
{
	_miner.calculateNextWorkRequiredWithDeposit
	(
		nAverageBits,
		nTimeUsed,
		nTimeStandard,
		dblDeposit,
		uBombExplodingRoundIndex,
		uRoundIndex,
		function( err, oData )
		{
			//
			//	oData
			//	{ bits: 527258964, shiftByDeposit: 0, shiftByRoundIndex: 0 }
			//
			if ( err )
			{
				return console.error( err );
			}

			oData.nAverageBits		= nAverageBits;
			oData.nTimeUsed			= nTimeUsed;
			oData.nTimeStandard		= nTimeStandard;
			//oData.uBombExplodingRoundIndex	= uBombExplodingRoundIndex;
			oData.uRoundIndex		= uRoundIndex;

			//	...
			let sLine	= ``
			+ String( uRoundIndex ).padEnd( 10 )
			+ `|` + String( nAverageBits ).padStart( 10 )
			+ `|` + String( nTimeUsed ).padStart( 10 )
			+ `|` + String( nTimeStandard ).padStart( 10 )
			+ `|` + String( oData.bits ).padStart( 10 )
			+ `|` + String( oData.shiftByDeposit ).padStart( 16 )
			+ `|` + String( oData.shiftByRoundIndex ).padStart( 16 )
			;
			console.log( sLine );
		}
	);
}
