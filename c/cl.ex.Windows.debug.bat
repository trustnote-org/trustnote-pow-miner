mkdir -p output
cl /Wall /c /Zi /D "_DEBUG" /I "include/" ./include/crypto/blake2b.c
cl /Wall /c /Zi /D "_DEBUG" /I "include/" ./include/utility.cpp
cl /Wall /c /Zi /D "_DEBUG" /I "include/" ./include/uint256.cpp
cl /Wall /c /Zi /D "_DEBUG" /I "include/" ./include/utilstrencodings.cpp
cl /w /c /Zi /EHsc /D "WIN32" /D "_DEBUG" /I "include/" ./include/arith_uint256.cpp
cl /w /c /Zi /EHsc /D "WIN32" /D "_DEBUG" /I "include/" ./miner.cpp

link /OUT "./output/miner.Windows.debug" blake2b.obj utility.obj uint256.obj utilstrencodings.obj arith_uint256.obj miner.obj ../asm/equihash/Windows/equihash_avx2.obj
