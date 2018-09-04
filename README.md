# TrustNote Pow-Miner 

## Install
```
$ npm install trustnote-pow-miner
```


## Supported Platforms

MacOS, Linux, Windows


## Documentation

#### start mining
```js
const _miner	= require( 'trustnote-pow-miner' );

let _oOptions	=
	{
		bufInputHeader	: new Buffer( 140 ),
		difficulty	: _miner.difficulty256HexToUInt32( "0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff" ),
		calcTimes	: 30,
		maxLoop		: 1000000,
	};

_miner.startMining( _oOptions, function( err, oData )
{
	if ( null === err )
	{
		if ( oData )
		{
			if ( oData.win )
			{
				console.log( `WINNER WINNER, CHICKEN DINNER!`, oData );
			}
			else if ( oData.gameOver )
			{
				console.log( `GAME OVER!` );
			}
		}
		else
		{
			console.log( `INVALID DATA!` );
		}
	}
	else
	{
		console.log( `OCCURRED ERROR : `, err );
	}
});
```


### stop mining
```js
const _miner	= require( 'trustnote-pow-miner' );

_miner.stop();
console.log( `The KILL signals were sent to all workers.` );
```


### check proof of work
```js
const _miner	= require( 'trustnote-pow-miner' );

```