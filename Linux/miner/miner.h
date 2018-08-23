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


#ifndef OUT
	#define OUT
#endif


/**
 *	constants
 */
#define TRUSTNOTE_MINER_CONTEXT_SIZE	178033152
#define TRUSTNOTE_MINER_POW_LIMIT	"0fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"






/**
 *	start mining
 *
 *	@param	{uint8_t*}	putInputHeader
 *	@param	{uint32_t}	uDifficulty
 *	@param	{uint32_t}	uNonceStart
 *	@param	{uint32_t}	uNonceTimes
 *	@param	{uint32_t *}	puNonce
 *	@param	{char *}	pcszHash
 *	@return	{int}
 */
int startMining( uint8_t * putInputHeader, uint32_t uDifficulty, uint32_t uNonceStart, uint32_t uNonceTimes, OUT uint32_t * puNonce, OUT char * pszHash );


/**
 *	stop mining
 *	@return	{int}
 */
int stopMining();


/**
 *	check proof of work
 *
 *	@param	{uint8_t *}	putInputHeader
 *	@param	{uint32_t}	uDifficulty
 *	@param	{uint32_t}	uNonce
 *	@param	{const char *}	pcszHash
 *	@return	{int}
 *		0	- okay
 */
int checkProofOfWork( uint8_t * putInputHeader, uint32_t uDifficulty, uint32_t uNonce, const char * pcszHash );



/**
 *	filter difficulty
 *
 *	@param	{const char *}	pcszHash
 *	@param	{uint32_t}	uDifficulty
 *	@param	{const char *}	pcszPowLimit
 *	@return	{int}
 *		0	- matched
 */
int filterDifficulty( uint32_t uDifficulty, const char * pcszHash, const char * pcszPowLimit = NULL );


/**
 *	calculate next difficulty
 *
 *	@param	{uint32_t}	uPreviousDifficulty
 *	@param	{uint32_t}	uTimeUsed
 *	@param	{uint32_t}	uTimeStandard
 *	@param	{const char * }	pcszPowLimit
 *	@return	{uint32_t}
 */
uint32_t calculateNextDifficulty( uint32_t uPreviousDifficulty, uint32_t uTimeUsed, uint32_t uTimeStandard, const char * pcszPowLimit );






#ifdef __cplusplus
}
#endif




#endif	//	__TRUSTNOTE_MINER_HEADER__
