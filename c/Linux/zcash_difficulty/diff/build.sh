g++ -o libdiff_adjust.so -std=c++11 -I. -I./comat -I./crypto -fPIC -shared diff_adjust.cpp  params.h uint256.h utilstrencodings.cpp arith_uint256.cpp uint256.cpp
