all: \
    trustnote-miner-deposit.o \
    deposit_table.o \
    deposit_table


trustnote-miner-deposit.o:
	g++ -std=c++11 -fPIC -c -g -D _DEBUG -I. -I../../include/ ../../include/trustnote-miner-deposit.cpp

deposit_table.o:
	g++ -std=c++11 -fPIC -c -g -D _DEBUG -I. -I../../include/ ./deposit_table.cpp

deposit_table:
	g++ -fPIC -g \
	    -o deposit_table \
	    ./deposit_table.o ./trustnote-miner-deposit.o



.PHONY: clean
clean:
	rm -rf deposit_test deposit_table *.o *.so *.a
