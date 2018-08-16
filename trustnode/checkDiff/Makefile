all: libproof.so test


libproof.so: proof.cpp
	g++ -std=c++11 -fPIC -shared -I. -I./crypto -o libproof.so proof.cpp uint256.cpp utilstrencodings.cpp arith_uint256.cpp
	cp libproof.so ~/.so

test: test.c
	gcc test.c -O2 -o test -I./ -L./ -lproof

.PHONY: clean
clean:
	-rm test libproof.so
