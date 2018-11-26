all: \
	trustnote-difficulty-bomb.o \
	test_difficulty_bomb.o \
	test_difficulty_bomb \
	clean_temp_file



trustnote-difficulty-bomb.o:
	g++ -std=c++11 -fPIC -c -g -D _DEBUG -I. -I../../include/ ../../include/trustnote-difficulty-bomb.cpp

test_difficulty_bomb.o:
	g++ -std=c++11 -fPIC -c -g -D _DEBUG -I. -I../../include/ ./test_difficulty_bomb.cpp


test_difficulty_bomb:
	g++ -fPIC -g \
	    -o test_difficulty_bomb \
	    ./trustnote-difficulty-bomb.o ./test_difficulty_bomb.o

clean_temp_file:
	rm -f *.o




.PHONY: clean
clean:
	rm -rf test_difficulty_bomb *.o *.so *.a
