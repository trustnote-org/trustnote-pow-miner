//
// Created by Liu QiXing on 2018/10/25.
//

#ifndef __TRUSTNOTE_MINER_TEST_HEADER__
#define __TRUSTNOTE_MINER_TEST_HEADER__




void miner_test_print_header( const char * pcszHeader );
void miner_test_print_footer( const char * pcszFooter = NULL );

void miner_test_checkProofOfWork();
void miner_test_getLimitInBits();
void miner_test_getLimitInTarget();
void miner_test_calculateNextWorkRequired();
void miner_test_startMining();




#endif //	__TRUSTNOTE_MINER_TEST_HEADER__
