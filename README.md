[![CircleCI](https://img.shields.io/badge/trustnote--pow--miner-build%20passed-green.svg)](https://github.com/trustnote/trustnote-pow-miner)
[![CircleCI](https://img.shields.io/badge/trustnote--pow--miner-test%20passed-brightgreen.svg)](https://github.com/trustnote/trustnote-pow-miner)



## TrustNote

TrustNote is an open source project that provides reliable and trusted public blockchain network services.            This project is supported by the TrustNote Foundation.

For more information, please visit our official website at [trustnote.org](https://trustnote.org/).

<br />


## About

TrustNote PoW-Miner is a free open source `TrustNote` miner for MacOS, Linux, Windows(x64) with multi-cores CPU support. It is written in C++ and has been tested on AMD/Intel CPUs for MacOS, Linux and Windows(x64).

<br />

## Platforms

All supported platforms:

* MacOS
* Linux
* Windows(x64)

<br />


## Run

#### Command line edition
> TrustNote PoW-Miner had been successfully compiled and executed on MacOS, Linux, Windows(x64). <br />
> But, TrustNote PoW-Miner in command line edition is only for unit testing and is not currently being used in any production environment.

#### Working with Node.js

> TrustNote PoW-Miner can be compiled into both executable binary file and shared object binary file. So, you can not only run the miner as a command line program, but also run it as a Node.js program by calling the export methods in JavaScript.

<br />


## Install
```
$ npm install https://github.com/trustnote/trustnote-pow-miner.git
```
<br />

## Compile

#### 1, Get source code

Clone the source code from github

```
$ git clone https://github.com/trustnote/trustnote-pow-miner.git
```


#### 2, Compiling

Change your current working directory to `/c`, and then, select a Makefile by target platform/OS you want to compile for.

| Makefile | Platform | Edition | output library file | passed on
|-------------|----------|----------|----------|----------|
| Makefile.dylib.Mac.debug | MacOS | debug  | miner.Mac.debug.dylib | MacOS Mojave
| Makefile.dylib.Mac.release | MacOS | release | miner.Mac.release.dylib | MacOS Mojave
| Makefile.so.Linux.debug | Linux | debug | miner.Linux.debug.so | Ubuntu 16.04.5
| Makefile.so.Linux.release | Linux | release | miner.Linux.release.so | Ubuntu 16.04.5
| cl.dll.Windows.x64.debug.bat | Windows x64 | debug | miner.Windows.x64.debug.dll | Windows 10 x64
| cl.dll.Windows.x64.release.bat | Windows x64 | release | miner.Windows.x64.release.dll | Windows 10 x64


#### 3, Working with Node.js

After compiling, all library files will be made in folder `/c/output/`. To use the exported methods in JavaScript, you need copy the output library files to `/lib/{platform/OS}/`


#### 4, Examples on how to compile

* MacOS

```bash
# compile debug edition
$ make clean
$ make -f Makefile.dylib.Mac.debug

# remove previous version
$ rm ../lib/Mac/miner.Mac.debug.dylib

# move the library file you just compiled to directory /lib/Mac/
$ mv ./output/miner.Mac.debug.dylib ../lib/Mac/

# Done!

```

* Linux

```bash
# compile debug edition
$ make clean
$ make -f Makefile.so.Linux.debug

# remove previous version
$ rm ../lib/Linux/miner.Linux.debug.so

# move the library file you just compiled to directory /lib/Linux/
$ mv ./output/miner.Linux.debug.so ../lib/Linux/

# Done!

```


* Windows(x64)

> **Required**:    
> Microsoft Visual Studio 2017(Community Edition). Please feel free to [Download](https://visualstudio.microsoft.com/vs/) and work with, it's free for our jobs.

1, Open `Visual Studio 2017` > `x64 Native Tools Command Prompt for VS 2017` from `Windows Start` button.    
2, Change your current working directory to `/trustnote-pow-miner/c/`


```bash
; compile debug edition
$ cl.clean.bat
$ cl.dll.Windows.x64.debug.bat

; remove previous version
$ del ../lib/Windows/miner.Windows.x64.debug.dll

; move the dll file you just compiled to directory /lib/Windows/
$ copy ./output/miner.Windows.x64.debug.dll ../lib/Windows/miner.Windows.x64.debug.dll

; Done!

```
<br />
<br />




## Documentation for Node.js

### .startMining( oOptions, pfnCallback )
start mining

* Input parameters

| name | type | optional | default | comment |
|--- | --- | --- | --- | --- |
| oOptions.bufInputHeader | Buffer | no | null | 140 bytes Buffer object |
| oOptions.bits | Number | no | null | | target value in 4 bytes |
| oOptions.calcTimes | Number | yes | 30 | compute times per loop |
| oOptions.maxLoop | Number | yes | 10000000 | max loop |
| oOptions.maxWorkerCount | Number | yes | _getDefaultMaxWorkerCount() | max worker count |
| pfnCallback	| function | no | null | callback function( err, oSolution ) |

oSolution is a plain object :

```JSON    
//	win
{ win : true, hashHex : sActualHashHex, nonce : uActualNonce }

//	game is over
{ win : false, gameOver : true, hashHex : null, nonce : 0 } 

```

* Return value

| return type | comment |
|----------|----------|----------|
| * | any

<br />


#### Demo


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



### .stopMining()

stop mining

* No input parameters

* Return value

| return type | comment |
|----------|----------|----------|
| * | any


```js
const _miner	= require( 'trustnote-pow-miner' );

_miner.stop();
console.log( `The KILL signals were sent to all workers.` );
```
<br />


### .checkProofOfWork( bufInputHeader, uBits, uActualNonce, sActualHashHex, pfnCallback )
check proof of work

* Input parameters

| parameter | type | comment |
|----------|----------|----------|
| bufInputHeader	| Buffer | 140 bytes Buffer object
| uBits	| Number | 4 bytes unsigned int
| uActualNonce	| Number | 4 bytes unsigned int
| sActualHashHex	| String | 32 bytes hex string, 64 characters
| pfnCallback	| function | callback function( err, { code : nCallCheckProofOfWork } )


* Return value

| return type | comment |
|----------|----------|----------|
| * | any

<br />





### .calculateNextWorkRequired( uPreviousBits, uTimeUsed, uTimeStandard, pfnCallback )

calculate target in bits required in the next work

* Input parameters

| parameter | type | comment |
|----------|----------|----------|
| uPreviousBits	| Number | target value in 4 bytes bits format of previous cycle
| uTimeUsed	| Number | actual used time value of previous cycle
| uTimeStandard	| Number | standard time of a cycle
| pfnCallback	| function | callback function( err, { bits : uNextBits } )

* Return value

| return type | comment |
|----------|----------|----------|
| * | any


<br />


### .calculateNextWorkRequiredWithDeposit( uPreviousBits, uTimeUsed, uTimeStandard, dblDeposit, uRoundIndex, pfnCallback )

calculate target in bits with deposit required in the next work

* Input parameters

| parameter | type | comment |
|----------|----------|----------|
| uPreviousBits	| Number | target value in 4 bytes bits format of previous cycle
| uTimeUsed	| Number | actual used time value of previous cycle
| uTimeStandard	| Number | standard time of a cycle
| dblDeposit	| Number | amount in Notes calculated by a deposit address of a TrustNote SuperNode
| uRoundIndex	| Number | current round index
| pfnCallback	| function | callback function( err, objResult )

objResult is a plain object:

```JSON        
{    
	// 4 bytes bits value
	bits : uNextBits,

	// bits to shift left by deposit
	shiftByDeposit : nShiftByDeposit,

	// bits to shift right by round index
	shiftByRoundIndex : nShiftByRoundIndex,
}
```

<br />


### .getBitsByTarget( sTarget256Hex )

get 4 bytes bits value by 32 bytes target hex string.

* Input parameters

| parameter | type | comment |
|----------|----------|----------|
| sTarget256Hex	| Srring | target value in 32 bytes hex string


sTarget256Hex string like:

```
00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
```

* Return value

| return type | comment |
|----------|----------|----------|
| Number | target value in 4 bytes bits value, e.g.: 520617983

<br />


### .getTargetByBits( uBits, pfnCallback )

get 32 bytes target hex string by 4 bytes bits value.

* Input parameters

| parameter | type | comment |
|----------|----------|----------|
| uBits	| Number | 4 bytes unsigned int
| pfnCallback	| function | callback function( err, sTargetHex )

uBits value like:

```
520617983
```

sTargetHex string like:

```
00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
```

* Return value

| return type | comment |
|----------|----------|----------|
| * | any


<br />
