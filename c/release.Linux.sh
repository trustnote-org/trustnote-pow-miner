rm ../lib/Mac/miner.Linux.*

make clean
make -f Makefile.so.Linux.debug
mv output/miner.Linux.debug.so ../lib/Linux

make clean
make -f Makefile.so.Linux.release
mv output/miner.Linux.release.so ../lib/Linux

