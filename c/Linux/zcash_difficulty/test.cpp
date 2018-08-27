#include <stdio.h>
#include <ctime>
#include <map>

#include "params.h"
#include "uint256.h"
#include "arith_uint256.h"

using namespace std;

#define random(a,b) (rand()%(b-a+1)+a)

unsigned int CalculateNextWorkRequired(arith_uint256 bnAvg,
                                       int64_t nLastBlockTime, int64_t nFirstBlockTime,
                                       const Consensus::Params& params);


class CBlockIndex{
	public:
		CBlockIndex* pprev;
		int nHeight;
		uint32_t nTime;
	    uint32_t nBits;

	enum { nMedianTimeSpan=11 };

	int64_t GetBlockTime() const
    {
        return (int64_t)nTime;
    }
	
	int64_t GetMedianTimePast() const {
		int64_t pmedian[nMedianTimeSpan];
        int64_t* pbegin = &pmedian[nMedianTimeSpan];
        int64_t* pend = &pmedian[nMedianTimeSpan];

        const CBlockIndex* pindex = this;
        for (int i = 0; i < nMedianTimeSpan && pindex; i++, pindex = pindex->pprev)
            *(--pbegin) = pindex->GetBlockTime();

        // sort(pbegin, pend);
        return pbegin[(pend - pbegin)/2];
	}
};


int64_t GetMedianTimePast(const CBlockIndex* pindex){

}


int checkProofOfWork(const char* sHash, int nBits, const char* sPowLimit)
{
	uint256 powLimit = uint256S(sPowLimit);
	uint256 hash = uint256S(sHash);

	bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

	// Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(powLimit)){
		printf("nBits below minimum work.\n");
		return -1;
	}

	// Check proof of work matches claimed amount
    if (UintToArith256(hash) > bnTarget){
		printf("hash doesn't match nBits.\n");
		return -1;
	}
	
	return 0;
}



unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast , const Consensus::Params& params)
{
    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();

    // Genesis block
    if (pindexLast == NULL)
        return nProofOfWorkLimit;

    // Find the first block in the averaging interval
    const CBlockIndex* pindexFirst = pindexLast;
    arith_uint256 bnTot {0};
	bool fNegative;
    for (int i = 0; pindexFirst && i < params.nPowAveragingWindow; i++) {	// nPowAveragingWindow = 17
        arith_uint256 bnTmp;
        bnTmp.SetCompact(pindexFirst->nBits);
        bnTot += bnTmp;
	//printf("bnTot = 0x%x\tnBits = 0x%x\n", bnTot.GetCompact(fNegative), pindexFirst->nBits);
        pindexFirst = pindexFirst->pprev;
    }

    // Check we have enough blocks
    if (pindexFirst == NULL)
        return nProofOfWorkLimit;

    arith_uint256 bnAvg {bnTot / params.nPowAveragingWindow};

    return CalculateNextWorkRequired(bnAvg, pindexLast->GetMedianTimePast(), pindexFirst->GetMedianTimePast(), params);
}



unsigned int CalculateNextWorkRequired(arith_uint256 bnAvg,
                                       int64_t nLastBlockTime, int64_t nFirstBlockTime,
                                       const Consensus::Params& params)
{
    // Limit adjustment step
    // Use medians to prevent time-warp attacks
    int64_t nActualTimespan = nLastBlockTime - nFirstBlockTime;
    printf("nActualTimespan = %d  before dampening\n", nActualTimespan);
	// 3/4 AveragingWindowTimespan + 1/4 nActualTimespan
    nActualTimespan = params.AveragingWindowTimespan() + (nActualTimespan - params.AveragingWindowTimespan())/4;
    printf("nActualTimespan = %d  before bounds\n", nActualTimespan);

    if (nActualTimespan < params.MinActualTimespan())	//	16% adjustment up
        nActualTimespan = params.MinActualTimespan();
    if (nActualTimespan > params.MaxActualTimespan())	// 	32% adjustment down
        nActualTimespan = params.MaxActualTimespan();

    // Retarget
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    arith_uint256 bnNew {bnAvg};
    bnNew /= params.AveragingWindowTimespan();
    bnNew *= nActualTimespan;

    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    /// debug print
    printf("GetNextWorkRequired RETARGET\n");
    printf("params.AveragingWindowTimespan() = %d    nActualTimespan = %d\n", params.AveragingWindowTimespan(), nActualTimespan);
    printf("Current average: %08x  %s\n", bnAvg.GetCompact(), bnAvg.ToString());
    printf("After:  %08x  %s\n", bnNew.GetCompact(), bnNew.ToString());

    return bnNew.GetCompact();
}



int main(int argc, char** argv){

	int iMin = atoi(argv[1]);
	int iMax = atoi(argv[2]);

	arith_uint256 bnAvg;
	srand((unsigned)time(0));
	map<uint32_t, CBlockIndex> blocks;
	Consensus::Params consensus;
	{
		consensus.powLimit = uint256S("0007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
		consensus.nPowAveragingWindow = 17;
		consensus.nPowTargetSpacing = 150;	// 2.5 * 60
		consensus.nPowMaxAdjustUp = 16;
		consensus.nPowMaxAdjustDown = 32;
	}
	
	for (uint32_t i = 0; i <= 10; i++) {
        blocks[i].pprev = i ? &blocks[i - 1] : NULL;
        blocks[i].nHeight = i;
        blocks[i].nTime = i ? blocks[i-1].nTime + random(iMin, iMax) : 1269211443;//1269211443 + i * consensus.nPowTargetSpacing;
        blocks[i].nBits = i ? GetNextWorkRequired(&blocks[i-1], consensus) : UintToArith256(consensus.powLimit).GetCompact();
        printf("%d\t%d\t%d\t0x%x\n", blocks[i].nTime,
			i ? blocks[i].nTime - blocks[i-1].nTime - consensus.nPowTargetSpacing : 0,
			i ? blocks[i].nTime - blocks[0].nTime - i * consensus.nPowTargetSpacing : 0,
			blocks[i].nBits);
    }
	return 0;
}




