//process.env.ENV_TRUST_MINER_DEBUG = true;


const _miner	= require( '../js/index.js' );
const _async	= require( 'async' );



console.log( `-------------------------------------------------------` );
console.log( `${ 'ADJUST'.padEnd( 6 ) } :  100% | Previous  | TimeUsed | new difficult |` );
console.log( `-------------------------------------------------------` );


let _arrComputeFunctions	= [];
let _nDefaultAverageBits	= 528482303;	//528482303;

for ( let i = 9000; i > 0; i -= 2 )
{
	_arrComputeFunctions.push
	(
		function( pfnNext )
		{
			calculateNext( _nDefaultAverageBits, i, function( err, uBits )
			{
				if ( err )
				{
					return pfnNext( err );
				}

				let sFlag	= '';
				let sAdjust	= '';
				let sOperator	= uBits > _nDefaultAverageBits
						? '+'
						: uBits === _nDefaultAverageBits
							? '-'
							: '-';
				if ( uBits > _nDefaultAverageBits )
				{
					sFlag	= 'EASIER';
					sAdjust	= ( ( Math.abs( uBits - _nDefaultAverageBits ) * 100 ) / _nDefaultAverageBits ).toFixed( 2 );
				}
				else if ( uBits < _nDefaultAverageBits )
				{
					sFlag	= 'HARDER';
					sAdjust	= ( ( Math.abs( uBits - _nDefaultAverageBits ) * 100 ) / _nDefaultAverageBits ).toFixed( 2 );
				}
				else
				{
					sFlag	= 'SAME';
					sAdjust	= '----';
				}

				let sTimeUsed	= String( i ) + '';
				let sOutput	= `${ sFlag.padEnd( 6 ) } | ${ sOperator }${ sAdjust } | ${ _nDefaultAverageBits } | ${ sTimeUsed.padStart( 8 ) } | ${ uBits }`;
				console.log( sOutput );

				//	...
				_nDefaultAverageBits = uBits;

				//	...
				pfnNext();
			});
		}
	);
}


_async.series
(
	_arrComputeFunctions,
	function( err )
	{
		if ( err )
		{
			return console.error( err );
		}

		console.log( `Done!` );
	}
);





//
//	calculate next bits
//
function calculateNext( nAverageBits, nTimeUsed, pfnCallback )
{
	let nTimeStandard	= 2400;
	_miner.calculateNextWorkRequired
	(
		nAverageBits,
		nTimeUsed,
		nTimeStandard,
		function( err, oData )
		{
			//
			//	oData
			//	{ bits : uNextBits }
			//
			if ( err )
			{
				return pfnCallback( err );
			}

			if ( oData &&
				'object' === typeof oData )
			{
				if ( oData.hasOwnProperty( 'bits' ) &&
					'number' === typeof oData.bits &&
					oData.bits > 0 )
				{
					pfnCallback( null, oData.bits );
				}
				else
				{
					pfnCallback( `calculateNextWorkRequired callback :: invalid value .bits` );
				}
			}
			else
			{
				pfnCallback( `calculateNextWorkRequired callback :: invalid oData object` );
			}
		}
	);
}
