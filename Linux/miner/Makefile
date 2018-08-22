all: \
    blake2b.o \
    utility.o \
    libcrypto.a \
    miner.so


blake2b.o:
	gcc -fPIC -c -I. -I../include/ ../include/crypto/blake2b.c

utility.o:
	g++ -fPIC -c -I. -I../include/ ../include/utility.cpp

libcrypto.a:
	g++ -std=c++11 -fPIC -c -g -I. -I../include/ \
        ../include/uint256.cpp \
        ../include/utilstrencodings.cpp \
        ../include/arith_uint256.cpp
	ar -crv libcrypto.a uint256.o utilstrencodings.o arith_uint256.o blake2b.o




miner.so:
	g++ -std=c++11 -fPIC -shared -I. -I../include/ \
	    -o miner.so \
	    ./miner.cpp \
	    ./utility.o ./libcrypto.a ../library/equihash_avx2.o





.PHONY: clean
clean:
	rm test miner *.o *.a
