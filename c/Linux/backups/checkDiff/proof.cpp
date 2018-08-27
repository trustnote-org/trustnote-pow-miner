
#include "params.h"
#include "uint256.h"
#include "arith_uint256.h"
#include "proof.h"

// 校验结果值hash是否符合难度要求
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

