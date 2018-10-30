# TrustNote Pow-Miner 

## Install
```
$ npm install trustnote-pow-miner
```


## Supported Platforms

MacOS, Linux, Windows


## Documentation

### .startMining( oOptions, pfnCallback )
start mining

* oOptions

| name | type | optional | comment |
|--- | --- | --- | --- |
| .bufInputHeader | Buffer | no | 140 bytes Buffer object |
| .bits | Number | no | number |
| .calcTimes | Number | yes | default value : 30, compute times per loop |
| .maxLoop | Number | yes | default value : 10000000, max loop |

 * pfnCallback( err, oSolution )

> oSolution is a plain object :

> { win : true, hashHex : sActualHashHex, nonce : uActualNonce } <br />
> { win : false, gameOver : true, hashHex : null, nonce : 0 } 


```js
const _miner	= require( 'trustnote-pow-miner' );

/**
 *	
 */
let bufInput	= new Buffer( 140 );
let uBits	= _miner.getBitsByTarget( "0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff" );
let oOptions	=
	{
		bufInputHeader	: bufInput,
		bits		: uBits,
		calcTimes	: 30,
		maxLoop		: 1000000,
	};
_miner.startMining( oOptions, function( err, oData )
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
<br />
<br />
<br />



### .stopMining()

stop mining


```js
const _miner	= require( 'trustnote-pow-miner' );

_miner.stop();
console.log( `The KILL signals were sent to all workers.` );
```
<br />
<br />
<br />


### .checkProofOfWork( bufInputHeader, uBits, uActualNonce, sActualHashHex, pfnCallback )
check proof of work









```js
const _miner	= require( 'trustnote-pow-miner' );


checkProofOfWork( bufInputHeader, uBits, uActualNonce, sActualHashHex, pfnCallback )

```