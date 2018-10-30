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


#ifndef IN
	#define IN
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
#define TRUSTNOTE_MINER_POW_LIMIT_TARGET	"0007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
#define TRUSTNOTE_MINER_POW_LIMIT_BITS		520617983





/**
 *	start mining
 *
 *	@param	{uint8_t*}	pcutInputHeader		input value in 140 bits format
 *	@param	{uint32_t}	uBits			target value in 32 bits format
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
 *	@param	{uint32_t}	uBits			target value in 32 bits format
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
 *	calculate next work required target in 32 bits format
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


/**
 *	calculate next work required target in 32 bits format
 *
 *	@param	{uint32_t}	uPreviousBits
 *	@param	{uint32_t}	uTimeUsed
 *	@param	{uint32_t}	uTimeStandard
 *	@param	{double}	dblDeposit
 *	@param	{uint32_t}	uRoundIndex
 *	@return	{uint32_t}
 */
EXPORT_API uint32_t calculateNextWorkRequiredWithDeposit(
	const uint32_t uPreviousBits,
	const uint32_t uTimeUsed,
	const uint32_t uTimeStandard,
	const double   dblDeposit,
	const uint32_t uRoundIndex );


/**
 *	check if the uBits is easier than limit
 *
 *	@param	{uint32_t}	uBits		e.g.: 0x1c03a809
 *	@return	{bool}
 */
EXPORT_API bool isEasierThanLimitByBits( const uint32_t uBits );


/**
 *	get work required target limit in format string 256 bits
 *
 *	@return	{int}				e.g.: "00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
 */
EXPORT_API int getLimitInTarget( OUT char * pszTargetHex, uint32_t uSize );


/**
 *	get work required target limit in format unsigned int 32 bits
 *
 *	@return	{uint32_t}			e.g.: 0x1c03a809
 */
EXPORT_API uint32_t getLimitInBits();


/**
 *	convert 256 bits string target to 32 bits uint32_t bits
 *
 *	@param 	{const char *}	pcszTargetHex	e.g.: "00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
 *	@return	{uint32_t}			e.g.: 0x1c03a809
 */
EXPORT_API uint32_t getBitsByTarget( const char * pcszTargetHex );


/**
 *	convert 32 bits uint32_t bits to 256 bits string target
 *
 *	@param 	{uint32_t}	uBits		e.g.: 0x1c03a809
 *	@param 	{char *}	pszTargetHex	e.g.: "00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
 *	@param 	{uint32_t}	uSize
 *	@return	{int}
 */
EXPORT_API int getTargetByBits( uint32_t uBits, OUT char * pszTargetHex, uint32_t uSize );









#ifdef __cplusplus
}
#endif




#endif	//	__TRUSTNOTE_MINER_HEADER__
