#include "../../include/bitcoin-consensus-params.h"
#include "../../include/uint256.h"
#include "../../include/arith_uint256.h"
#include "proof.h"


/**
 * 	check hash with difficulty
 *
 *	@param	{string}	sHash
 *	@param	{int}		nDifficulty
 *	@param	{string}	sPowLimit
 *	@return	{integer}
 */
int checkProofOfWork(const char* sHash, int nDifficulty, const char* sPowLimit)
{
	uint256 powLimit = uint256S(sPowLimit);
	uint256 hash = uint256S(sHash);

	bool fNegative;
    	bool fOverflow;
    	arith_uint256 bnTarget;

    	bnTarget.SetCompact(nDifficulty, &fNegative, &fOverflow);

	// Check range
    	if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(powLimit)){
	//	printf("nBits below minimum work.\n");
		return -1;
	}

	// Check proof of work matches claimed amount
   	if (UintToArith256(hash) > bnTarget){
	//	printf("hash doesn't match nBits.\n");
		return -1;
	}
	
	return 0;
}

