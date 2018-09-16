//
// Created by LiuQixing on 2018/9/16.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <algorithm>

#include "difficulty.h"
#include "consensus.h"
#include "params.h"
#include "arith_uint256.h"
#include "uint256.h"


const arith_uint256 maxUint = UintToArith256(uint256S("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"));

unsigned int CalculateNextWorkRequired
	(
		arith_uint256 bnAvg,
		int64_t nLastBlockTime,
		int64_t nFirstBlockTime,
		const Consensus::Params &params );








int main()
{
	printf("Hello");

	Consensus::Params consensus;

	consensus.fCoinbaseMustBeProtected = true;
	consensus.nSubsidySlowStartInterval = 20000;
	consensus.nSubsidyHalvingInterval = 840000;
	consensus.nMajorityEnforceBlockUpgrade = 750;
	consensus.nMajorityRejectBlockOutdated = 950;
	consensus.nMajorityWindow = 4000;
	consensus.powLimit = uint256S("0007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
	consensus.nPowAveragingWindow = 17;
	assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
	consensus.nPowMaxAdjustDown = 32; // 32% adjustment down
	consensus.nPowMaxAdjustUp = 16; // 16% adjustment up
	consensus.nPowTargetSpacing = 2.5 * 60;
	consensus.vUpgrades[Consensus::BASE_SPROUT].nProtocolVersion = 170002;
	consensus.vUpgrades[Consensus::BASE_SPROUT].nActivationHeight =
		Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
	consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nProtocolVersion = 170002;
	consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
		Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
	consensus.vUpgrades[Consensus::UPGRADE_OVERWINTER].nProtocolVersion = 170005;
	consensus.vUpgrades[Consensus::UPGRADE_OVERWINTER].nActivationHeight = 347500;
	consensus.vUpgrades[Consensus::UPGRADE_SAPLING].nProtocolVersion = 170007;
	consensus.vUpgrades[Consensus::UPGRADE_SAPLING].nActivationHeight = 419200;

	// The best chain should have at least this much work.
	consensus.nMinimumChainWork = uint256S("0x00000000000000000000000000000000000000000000000000281b32ff3198a1");


	arith_uint256 bnAvg { 380 };
	unsigned int nNew = CalculateNextWorkRequired( bnAvg, 1500, 0, consensus );
}


unsigned int CalculateNextWorkRequired
(
	arith_uint256 bnAvg,					/* average time used in previous 17 blocks */
	int64_t nLastBlockTime, int64_t nFirstBlockTime,	/* actual time used */
	const Consensus::Params &params				/* standard time */
)
{
	//
	//	Limit adjustment step
	//	Use medians to prevent time-warp attacks
	int64_t nActualTimespan = nLastBlockTime - nFirstBlockTime;
	printf( "pow, nActualTimespan = %d  before dampening\n", nActualTimespan);
	//nActualTimespan = params.AveragingWindowTimespan() + ( nActualTimespan - params.AveragingWindowTimespan() )/4;
	nActualTimespan = (params.AveragingWindowTimespan() * 3 + nActualTimespan) / 4;
	printf( "pow, nActualTimespan = %d  before bounds\n", nActualTimespan);

	if (nActualTimespan < params.MinActualTimespan()) {
		nActualTimespan = params.MinActualTimespan();
	}
	if (nActualTimespan > params.MaxActualTimespan()) {
		nActualTimespan = params.MaxActualTimespan();
	}

	//	Retarget
	const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
	arith_uint256 bnNew{bnAvg};
	bnNew /= params.AveragingWindowTimespan();
	bnNew *= nActualTimespan;

	if (bnNew > bnPowLimit) {
		bnNew = bnPowLimit;
	}

	//	debug print
	printf( "pow, GetNextWorkRequired RETARGET\n");
	printf( "pow, params.AveragingWindowTimespan() = %d    nActualTimespan = %d\n",
		 params.AveragingWindowTimespan(), nActualTimespan);
	printf( "pow, Current average: %08x  %s\n", bnAvg.GetCompact(), bnAvg.ToString().c_str() );
	printf( "pow, After:  %08x  %s\n", bnNew.GetCompact(), bnNew.ToString().c_str() );

	return bnNew.GetCompact();
}

