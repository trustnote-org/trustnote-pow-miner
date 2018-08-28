#ifndef __DIFF_ADJUST
#define __DIFF_ADJUST

#include <stdio.h>
#include <stdint.h>

#include "params.h"
#include "uint256.h"
#include "arith_uint256.h"

#ifdef __cplusplus 
extern "C" {
#endif


/**
 *		difficult		历史难度，上一次的难度
 *		nActualTimespan		实际的执行时间
 *		nStandardTimespan	应该使用的时间： 150 * round
 *		sPowLimit		计算使用的最低难度值，常量，不需要修改
 *
 *		@return			>= 1
 */
uint64_t CalculateNextWorkRequired(uint64_t difficult,
                                   uint64_t nActualTimespan,
                                   uint64_t nStandardTimespan,
                                   const char* sPowLimit);

#ifdef __cplusplus 
}
#endif

#endif
