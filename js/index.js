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
	getBitsByTarget			: _miner.library.getBitsByTarget,
};
