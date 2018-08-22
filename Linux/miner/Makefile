all: \
    libproof.a \
    blake2b.o \
    quickbench.o \
    quickbench_avx2

    #quickbench_avx1 \
    #solver_avx1 \
    #solver_avx2 \


libproof.a: ./proof/proof.cpp
	g++ -std=c++11 -c -g -I. -I./proof/crypto/ ./proof/proof.cpp ./proof/uint256.cpp ./proof/utilstrencodings.cpp ./proof/arith_uint256.cpp
	ar -crv libproof.a proof.o uint256.o utilstrencodings.o arith_uint256.o

blake2b.o: ./crypto/blake2b.c
	gcc -c -I. ./crypto/blake2b.c

quickbench.o: ./quickbench.c
	gcc -c -I. ./quickbench.c




#solver_avx1: solver.c equihash_avx1.o
#	gcc -o solver_avx1 solver.c equihash_avx1.o
#
#solver_avx2: solver.c equihash_avx2.o
#	g++ ./libproof.a -o solver_avx2 solver.c equihash_avx2.o




#quickbench_avx1: quickbench.c equihash_avx1.o blake2b.o
#	g++ -std=c++11 -o quickbench_avx1 ./libproof.a quickbench.c ./equihash_avx1.o ./blake2b.o

quickbench_avx2: quickbench.o
	g++ -o quickbench_avx2 quickbench.o ./libproof.a ./blake2b.o ./equihash_avx2.o




.PHONY: clean
clean:
	rm test solver_avx1 solver_avx2 quickbench_avx1 quickbench_avx2 *.a libthreadpool.so blake2b.o proof.o uint256.o utilstrencodings.o arith_uint256.o libthreadpool.so
