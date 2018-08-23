const _ffi		        = require( 'ffi' );

let _objMinerLibrary    = null;



//int startMining( uint8_t * putInputHeader, uint32_t uDifficulty, uint32_t uNonceStart = 0, uint32_t uNonceTimes = 0 );

_objMinerLibrary = _ffi.Library
(
    `${ __dirname }/miner.so`,
    {
        'startMining': [ 'int',  [ 'pointer', 'uint', 'uint', 'uint'  ] ]
    }
);


_objMinerLibrary.startMining( new Buffer(140 ), 0, 0, 0 );