all: \
	utilstrencodings.o \
	uint256.o \
	arith_uint256.o \
	target_shift.o \
	target_shift


utilstrencodings.o:
	g++ -std=c++11 -fPIC -c -g -D _DEBUG -I. -I../../include/ ../../include/utilstrencodings.cpp

uint256.o:
	g++ -std=c++11 -fPIC -c -g -D _DEBUG -I. -I../../include/ ../../include/uint256.cpp

arith_uint256.o:
	g++ -std=c++11 -fPIC -c -g -D _DEBUG -I. -I../../include/ ../../include/arith_uint256.cpp

target_shift.o:
	g++ -std=c++11 -fPIC -c -g -D _DEBUG -I. -I../../include/ ./target_shift.cpp


target_shift:
	g++ -fPIC -g \
	    -o target_shift \
	    ./target_shift.o ./uint256.o ./arith_uint256.o ./utilstrencodings.o



.PHONY: clean
clean:
	rm -rf target_shift deposit_table *.o *.so *.a
