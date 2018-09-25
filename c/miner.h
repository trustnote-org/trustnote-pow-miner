/**
 * 	TrustNote Equihash PoW Miner
 *
 *
 *	Copyright @ TrustNote.org
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

#ifdef _WIN32
	#ifdef LIBRARY_EXPORTS
		#define LIBRARY_API __declspec(dllexport)
	#else
		#define LIBRARY_API __declspec(dllimport)
	#endif
#else
	#define LIBRARY_API
#endif




/**
 *	constants
 */
#define TRUSTNOTE_MINER_CONTEXT_SIZE		178033152
#define TRUSTNOTE_MINER_POW_MAX			"ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
#define TRUSTNOTE_MINER_POW_MIN			"0000000000000000000000000000000000000000000000000000000000000000"
#define TRUSTNOTE_MINER_POW_LIMIT		"007fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
//#define TRUSTNOTE_MINER_DIFFICULTY_START	0x1f07ffff	//	= 520617983





/**
 *	start mining
 *
 *	@param	{uint8_t*}	pcutInputHeader
 *	@param	{uint32_t}	uDifficulty
 *	@param	{uint32_t}	uNonceStart
 *	@param	{uint32_t}	uCalcTimes
 *	@param	{uint32_t *}	OUT puNonce
 *	@param	{char *}	OUT pszHashHex
 *	@param	{uint32_t}	uHashHexLength
 *	@return	{int}
 */
LIBRARY_API int startMining(
	const uint8_t * pcutInputHeader,
	const uint32_t uDifficulty,
	const uint32_t uNonceStart,
	const uint32_t uCalcTimes,
	OUT uint32_t * puNonce,
	OUT char * pszHashHex,
	uint32_t uHashHexLength );


/**
 *	stop mining
 *	@return	{int}
 */
LIBRARY_API int stopMining();


/**
 *	check proof of work
 *
 *	@param	{uint8_t *}	pcutInputHeader
 *	@param	{uint32_t}	uDifficulty
 *	@param	{uint32_t}	uNonce
 *	@param	{const char *}	pcszHashHex
 *	@return	{int}
 *		0	- okay
 */
LIBRARY_API int checkProofOfWork(
	const uint8_t * pcutInputHeader,
	const uint32_t uDifficulty,
	const uint32_t uNonce,
	const char * pcszHashHex );


/**
 *	convert 256 bits string to uint32_t
 *
 *	@param 	{const char *}	pcszDifficultyHex	"00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
 *	@return	{uint32_t}
 */
LIBRARY_API uint32_t difficulty256HexToUInt32( const char * pcszDifficultyHex );



/**
 *	filter difficulty
 *
 *	@param	{uint32_t}	uDifficulty
 *	@param	{const char *}	pcszHashHex
 *	@return	{int}
 *		0	- matched
 */
LIBRARY_API int filterDifficulty(
	const uint32_t uDifficulty,
	const char * pcszHashHex );



/**
 *	calculate next difficulty
 *
 *	@param	{uint32_t}	uPreviousDifficulty
 *	@param	{uint32_t}	uTimeUsed
 *	@param	{uint32_t}	uTimeStandard
 *	@return	{uint32_t}
 */
LIBRARY_API uint32_t calculateNextDifficulty(
	const uint32_t uPreviousDifficulty,
	const uint32_t uTimeUsed,
	const uint32_t uTimeStandard );






#ifdef __cplusplus
}
#endif




#endif	//	__TRUSTNOTE_MINER_HEADER__
