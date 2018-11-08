'use strict';

const _miner			= require( './miner.js' );



/**
 * 	@exports
 */
module.exports	= {
	startMining				: _miner.start,
	stopMining				: _miner.stop,
	checkProofOfWork			: _miner.library.checkProofOfWork,
	calculateNextWorkRequired		: _miner.library.calculateNextWorkRequired,
	calculateNextWorkRequiredWithDeposit	: _miner.library.calculateNextWorkRequiredWithDeposit,
	getBitsByTarget				: _miner.library.getBitsByTarget,
	getTargetByBits				: _miner.library.getTargetByBits,
};
