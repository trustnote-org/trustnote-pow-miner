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
	int res = checkProofOfWork
		(
			"007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
			1000,
			"007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
		);
	printf( "result : %d\n", res );

	return 0;
}
