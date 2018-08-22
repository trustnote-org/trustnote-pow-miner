/**
 * 	TrustNote Equihash PoW Miner
 *
 *
 *	Copyright @ TrustNote.com
 *
 * 	@history
 *	Created by Liu QiXing on 2018/8/22.
 */
#ifndef __TRUSTNOTE_MINER_HEADER__
#define __TRUSTNOTE_MINER_HEADER__


#ifdef __cplusplus
extern "C" {
#endif




/**
 *	constants
 */
#define TRUSTNOTE_MINER_CONTEXT_SIZE	178033152
#define TRUSTNOTE_MINER_POW_LIMIT	"007fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"



/**
 *	start mining
 *
 *	@param	{uint8_t*}	putInputHeader
 *	@param	{uint32_t}	nDifficulty
 *	@param	{uint32_t}	nNonceStart
 *	@param	{uint32_t}	nNonceTimes
 *	@return	{uint32_t}
 */
int startMining( uint8_t * putInputHeader, uint32_t nDifficulty, uint32_t nNonceStart = 0, uint32_t nNonceTimes = 0 );


/**
 *	check proof of work
 *
 *	@param	{const char *}	pcszHash
 *	@param	{uint32_t}	nDifficulty
 *	@param	{const char *}	pcszPowLimit
 *	@return	{uint32_t}
 */
int checkProofOfWork( const char * pcszHash, uint32_t nDifficulty, const char * pcszPowLimit = NULL );


/**
 *	calculate next difficulty
 *
 *	@param	{uint32_t}	nDifficulty
 *	@param	{uint32_t}	nTimeUsed
 *	@param	{uint32_t}	nTimeStandard
 *	@param	{const char * }	pcszPowLimit
 *	@return	{uint32_t}
 */
int calculateNextDifficulty( uint32_t nDifficulty, uint32_t nTimeUsed, uint32_t nTimeStandard, const char * pcszPowLimit );






#ifdef __cplusplus
}
#endif




#endif	//	__TRUSTNOTE_MINER_HEADER__
