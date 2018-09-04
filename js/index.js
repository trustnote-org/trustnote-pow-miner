'use strict';

const _miner			= require( './miner.js' );



/**
 * 	@exports
 */
module.exports	= {
	startMining			: _miner.start,
	stopMining			: _miner.stop,
	checkProofOfWork		: _miner.library.checkProofOfWork,
	calculateNextDifficulty		: _miner.library.calculateNextDifficulty,
	difficulty256HexToUInt32	: _miner.library.difficulty256HexToUInt32,
};
