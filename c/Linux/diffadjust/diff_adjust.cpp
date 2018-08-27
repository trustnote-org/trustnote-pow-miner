#include <stdio.h>
#include <stdint.h>
#include <ctime>
#include <map>

#include "diff_adjust.h"


uint64_t CalculateNextWorkRequired(uint64_t difficult,
                                        uint64_t nActualTimespan,
                                        uint64_t nStandardTimespan,
                                        const char* sPowLimit)
{

	uint256 powLimit = uint256S(sPowLimit);
	arith_uint256 bnTot {difficult}, bnPowLimit;
	uint64_t nActualTmspan;
	int64_t MinActualTimespan = (nStandardTimespan * ( 100 - 16 )) / 100;
	int64_t MaxActualTimespan = (nStandardTimespan * ( 100 + 32 )) / 100;

    	// 3/4 AveragingWindowTimespan + 1/4 nActualTimespan
    	nActualTmspan = nStandardTimespan + (nActualTimespan - nStandardTimespan)/4;
    
	if (nActualTmspan < MinActualTimespan)	//	84% adjustment up
        	nActualTmspan = MinActualTimespan;
	if (nActualTmspan > MaxActualTimespan)	// 	132% adjustment down
        	nActualTmspan = MaxActualTimespan;

	// Retarget
	bnPowLimit = UintToArith256(powLimit);
	printf("powLimit: %d\n", bnPowLimit.GetCompact());

    	arith_uint256 bnNew {bnTot};
    	bnNew /= nStandardTimespan;
    	bnNew *= nActualTmspan;

    	if (bnNew > bnPowLimit)
        	bnNew = bnPowLimit;

    	return bnNew.GetCompact();
}


