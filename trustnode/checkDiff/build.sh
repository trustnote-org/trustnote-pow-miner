g++ -std=c++11 -fPIC -shared -I. -I./crypto -o libequihash.so equihash.cpp uint256.cpp utilstrencodings.cpp arith_uint256.cpp


