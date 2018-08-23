//
// Created by Liu QiXing on 2018/8/22.
//





int main( void )
{
	FILE * fp;
	uint8_t utInputHeader[ 140 ];	//	140 byte header
	char szBuff[ 280 ];
	void * pvContextAlloc;
	void * pvContext;
	void * pvContextEnd;
	int32_t nSolutionCount;
	uint32_t nNonce;
	uint8_t utOutContext[ 32 ];

	//	...
	fp = fopen( "input.bin", "rb" );
	if ( ! fp )
	{
		puts( "input.bin not found, use sample data (beta1 testnet block 2)" );
		return -1;
	}

	//	...
	puts( "Reading input.bin" );
	fread( utInputHeader, 140, 1, fp );
	fclose( fp );

	//	...
	pvContextAlloc	= malloc( TRUSTNOTE_MINER_CONTEXT_SIZE + 4096 );
	pvContext	= (void*)( ( (long)pvContextAlloc + 4095 ) & -4096 );
	pvContextEnd	= pvContext + TRUSTNOTE_MINER_CONTEXT_SIZE;

	bytesToHexString( utInputHeader, 140, szBuff );
	printf( "input: %.*s\n", 280, szBuff );

	//	...
	nNonce = 0;
	for ( int i = 0; i < 10000000; i ++ )
	{
		EhPrepare( pvContext, (void *)utInputHeader );
		nSolutionCount	= EhSolver( pvContext, nNonce );
		for ( int n = 0; n < nSolutionCount; n ++ )
		{
			blake2b( (uint8_t*)utOutContext, (uint8_t*)pvContext + n * 1344, NULL, sizeof(utOutContext), 1344, 0 );
			bytesToHexString( utOutContext, 32, szBuff );

			int nCheck = filterDifficulty
				(
					szBuff,
					1000,
					"007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
				);
			printf( "[%d] - nonce: %d\t buff: %.*s\n", nCheck, nNonce, 64, szBuff );
			if ( 0 == nCheck )
			{
				printf( "Find it out." );
				break;
			}
		}

		nNonce ++;
	}

	free( pvContextAlloc );
	pvContextAlloc = NULL;

	//	...
	return 0;
}
