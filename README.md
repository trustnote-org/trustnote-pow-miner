[![CircleCI](https://img.shields.io/badge/trustnote--pow--miner-build%20passed-green.svg)](https://github.com/trustnote/trustnote-pow-miner)
[![CircleCI](https://img.shields.io/badge/trustnote--pow--miner-test%20passed-brightgreen.svg)](https://github.com/trustnote/trustnote-pow-miner)



## TrustNote

TrustNote is an open source project that provides reliable and trusted public blockchain network services.            This project is supported by the TrustNote Foundation.

For more information, please visit our official website at [trustnote.org](https://trustnote.org/).


## About

TrustNote PoW-Miner is a free open source `TrustNote` miner for MacOS, Linux, Windows(x64) with multi-CPU  support. It is written in C++ and has been tested with AMD/Intel CPUs on MacOS, Linux, Windows(x64).

## Platforms

All supported platforms:

* MacOS
* Linux
* Windows(x64)


## Run

#### Command line edition
> TrustNote PoW-Miner had been successfully compiled and executed on MacOS, Linux, Windows(x64). <br />
> But, TrustNote PoW-Miner in command line edition is only for unit testing and is not currently being used in any production environment.

#### Working with Node.js

> TrustNote PoW-Miner can be compiled into both executable binary file and shared object binary file. So, you can not only run the miner as a command line program, but also run it as a Node.js program by calling the export methods in JavaScript.


## Install
```
$ npm install https://github.com/trustnote/trustnote-pow-miner.git
```


## Compile

#### 1, Clone source code

Clone source code from github

```
$ git clone https://github.com/trustnote/trustnote-pow-miner.git
```

#### 2, Compile

Change your current working directory to `/c`, and then, select a Makefile by target platform/OS you want to compile for.

| Command | Platform/OS | Edition | library file | passed on
|-------------|----------|----------|----------|----------|
| make -f Makefile.dylib.Mac.debug | MacOS | debug  | miner.Mac.debug.dylib | MacOS Mojave
| make -f Makefile.dylib.Mac.release | MacOS | release | miner.Mac.release.dylib | MacOS Mojave
| make -f Makefile.so.Linux.debug | Linux | debug | miner.Linux.debug.so | Ubuntu 16.04.5 LTS
| make -f Makefile.so.Linux.release | Linux | release | miner.Linux.release.so | Ubuntu 16.04.5 LTS
| cl.dll.Windows.x64.debug.bat | Windows x64 | debug | miner.Windows.x64.debug.dll | Windows 10 x64
| cl.dll.Windows.x64.release.bat | Windows x64 | release | miner.Windows.x64.release.dll | Windows 10 x64


#### 3, Working with Node.js

After run compiling command, all library files will be made in folder `/c/output/`. To use the exported methods in JavaScript, you need copy the output library files to `/lib/{platform/OS}/`


#### 4, Examples on how to compile

* MacOS

```
# compile debug edition
$ make clean
$ make -f Makefile.dylib.Mac.debug

# remove previous version
$ rm ../lib/Mac/miner.Mac.debug.dylib

# move the library file you just compiled to directory /lib/Mac/
$ mv ./output/miner.Mac.debug.dylib ../lib/Mac/

# Done!

```
<br />

* Linux

```
# compile debug edition
$ make clean
$ make -f Makefile.so.Linux.debug

# remove previous version
$ rm ../lib/Linux/miner.Linux.debug.so

# move the library file you just compiled to directory /lib/Linux/
$ mv ./output/miner.Linux.debug.so ../lib/Linux/

# Done!

```
<br />


* Windows(x64)

> **Required**:    
> Microsoft Visual Studio 2017(Community Edition). Please feel free to [Download](https://visualstudio.microsoft.com/vs/) and work with, it's free for our jobs.

1, Open `Visual Studio 2017` > `x64 Native Tools Command Prompt for VS 2017` from `Windows Start` button.    
2, Change your current working directory to `/trustnote-pow-miner/c/`


```
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



### .stopMining()

stop mining


```js
const _miner	= require( 'trustnote-pow-miner' );

_miner.stop();
console.log( `The KILL signals were sent to all workers.` );
```
<br />


### .checkProofOfWork( bufInputHeader, uBits, uActualNonce, sActualHashHex, pfnCallback )
check proof of work









```js
const _miner	= require( 'trustnote-pow-miner' );


checkProofOfWork( bufInputHeader, uBits, uActualNonce, sActualHashHex, pfnCallback )

```




<br />
<br />
<br />
to be continued ...
<br />
<br />
<br />
