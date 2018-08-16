//compile with
//gcc -o quickbench quickbench.c equihash_avx2.o
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "proof.h"


int main(void)
{
	int res = checkProofOfWork("0007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
				   1,
			 	   "007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
	return 0;
}
