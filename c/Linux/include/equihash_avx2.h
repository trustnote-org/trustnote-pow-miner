//
// Created by Liu QiXing on 2018/8/21.
//

#ifndef TRUSTNOTE_POW_MINER_EQUIHASH_AVX2_H
#define TRUSTNOTE_POW_MINER_EQUIHASH_AVX2_H


#ifdef __cplusplus
extern "C" {
#endif


//
//	Linkage with assembly
//	EhPrepare takes in 136 bytes of input. The remaining 4 bytes of input is fed as nonce to EhSolver.
//	EhPrepare saves the 136 bytes in context, and EhSolver can be called repeatedly with different nonce.
//
void EhPrepare( void * context, void * input );
int32_t EhSolver( void * context, uint32_t nonce );





#ifdef __cplusplus
}
#endif



#endif //TRUSTNOTE_POW_MINER_EQUIHASH_AVX2_H
