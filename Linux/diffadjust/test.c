#include <stdio.h>
#include <stdint.h>

#include "diff_adjust.h"


int main(){

	unsigned int diff = CalculateNextWorkRequired(100, 100, 100, "007fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
	printf("diff = %d\n\n", diff);
	return 0;

}

