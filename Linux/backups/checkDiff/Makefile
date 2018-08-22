all: libproof.so libproof.a test


libproof.so: proof.cpp
	g++ -std=c++11 -fPIC -shared -I. -I./crypto -o libproof.so proof.cpp uint256.cpp utilstrencodings.cpp arith_uint256.cpp
	cp libproof.so ~/.so

libproof.a: proof.cpp
	g++ -std=c++11 -c -g -I. -I./crypto proof.cpp uint256.cpp utilstrencodings.cpp arith_uint256.cpp
	ar -crv libproof.a proof.o uint256.o utilstrencodings.o arith_uint256.o

test: test.c
	gcc test.c -O2 -o test1 -I./ -L./ -lproof
	g++ test.c -O2 ./libproof.a -o test2 -I./ -L./

.PHONY: clean
clean:
	-rm test test1 test2 *.so *.a *.o
