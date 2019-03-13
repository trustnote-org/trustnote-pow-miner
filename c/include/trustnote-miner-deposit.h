//
// Created by Liu QiXing on 2018/10/25.
//

#ifndef __TRUSTNOTE_POW_MINER_TRUSTNOTE_DEPOSIT_HEADER__
#define __TRUSTNOTE_POW_MINER_TRUSTNOTE_DEPOSIT_HEADER__


/**
 *	define the length of the table
 */
#define TRUSTNOTE_MINER_DEPOSIT_TABLE_LENGTH		33
#define TRUSTNOTE_MINER_DEPOSIT_DEFAULT_SHIFT		0
#define TRUSTNOTE_MINER_DEPOSIT_MIN_SHIFT		0
#define TRUSTNOTE_MINER_DEPOSIT_MAX_SHIFT		TRUSTNOTE_MINER_DEPOSIT_TABLE_LENGTH - 1
#define TRUSTNOTE_MINER_DEPOSIT_MAX_NOTES		500000000.0 * 1000.0 * 1000.0



/**
 *	structure
 */
typedef struct tagPowDeposit
{
	tagPowDeposit()
	{
		memset( this, 0, sizeof( tagPowDeposit ) );
	}
	tagPowDeposit( int nShift_, double dblTimes_, double dblDeposit_ )
	{
		nShift		= nShift_;
		dblTimes	= dblTimes_;
		dblDeposit	= dblDeposit_;
	}

	int    nShift;
	double dblTimes;
	double dblDeposit;

}STPOWDEPOSIT, *LPSTPOWDEPOSIT;





/**
 *	namespace of TrustNoteDeposit
 */
namespace TrustNoteDeposit
{
	int initDepositTable();

    	STPOWDEPOSIT * getDepositTable();
	int getShiftByDeposit( double dblDeposit );
	bool isValidShift( int nShift );
}





#endif //	__TRUSTNOTE_POW_MINER_TRUSTNOTE_DEPOSIT_HEADER__
