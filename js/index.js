'use strict';

const _miner			= require( './miner.js' );



/**
 * 	@exports
 */
module.exports	= {
	startMining			: _miner.start,
	stopMining			: _miner.stop,
	checkProofOfWork		: _miner.library.checkProofOfWork,
	calculateNextWorkRequired	: _miner.library.calculateNextWorkRequired,
	target256HexToBits32		: _miner.library.target256HexToBits32,
};
