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

#ifdef WIN32
	#define EXPORT_API __declspec(dllexport)
#else
	#define EXPORT_API
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
 *	@param	{uint32_t}	uBits
 *	@param	{uint32_t}	uNonceStart
 *	@param	{uint32_t}	uCalcTimes
 *	@param	{uint32_t *}	OUT puNonce
 *	@param	{char *}	OUT pszHashHex
 *	@param	{uint32_t}	uHashHexLength
 *	@return	{int}
 */
EXPORT_API int startMining(
	const uint8_t * pcutInputHeader,
	const uint32_t uBits,
	const uint32_t uNonceStart,
	const uint32_t uCalcTimes,
	OUT uint32_t * puNonce,
	OUT char * pszHashHex,
	uint32_t uHashHexLength );


/**
 *	stop mining
 *	@return	{int}
 */
EXPORT_API int stopMining();


/**
 *	check proof of work
 *
 *	@param	{uint8_t *}	pcutInputHeader
 *	@param	{uint32_t}	uBits
 *	@param	{uint32_t}	uNonce
 *	@param	{const char *}	pcszHashHex
 *	@return	{int}
 *		0	- okay
 */
EXPORT_API int checkProofOfWork(
	const uint8_t * pcutInputHeader,
	const uint32_t uBits,
	const uint32_t uNonce,
	const char * pcszHashHex );


/**
 *	convert 256 bits string to uint32_t
 *
 *	@param 	{const char *}	pcszDifficultyHex	"00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
 *	@return	{uint32_t}
 */
EXPORT_API uint32_t target256HexToBits32( const char * pcszDifficultyHex );



/**
 *	filter difficulty
 *
 *	@param	{uint32_t}	uBits
 *	@param	{const char *}	pcszHashHex
 *	@return	{int}
 *		0	- matched
 */
EXPORT_API int filterDifficulty(
	const uint32_t uBits,
	const char * pcszHashHex );



/**
 *	calculate next work required target in format bits
 *
 *	@param	{uint32_t}	uPreviousBits
 *	@param	{uint32_t}	uTimeUsed
 *	@param	{uint32_t}	uTimeStandard
 *	@return	{uint32_t}
 */
EXPORT_API uint32_t calculateNextWorkRequired(
	const uint32_t uPreviousBits,
	const uint32_t uTimeUsed,
	const uint32_t uTimeStandard );






#ifdef __cplusplus
}
#endif




#endif	//	__TRUSTNOTE_MINER_HEADER__
