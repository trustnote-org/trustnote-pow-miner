const CTrustMinerLibrary	= require( './CTrustMinerLibrary.js' );
const _miner			= require( './miner.js' );

/**
 * 	@variables
 */
let _oLibrary			= new CTrustMinerLibrary();



/**
 * 	@exports
 */
module.exports	= {
	startMining			: _miner.start,
	stopMining			: _miner.stop,
	checkProofOfWork		: _oLibrary.checkProofOfWork,
	calculateNextDifficulty		: _oLibrary.calculateNextDifficulty,
	difficulty256HexToUInt32	: _oLibrary.difficulty256HexToUInt32,
};
