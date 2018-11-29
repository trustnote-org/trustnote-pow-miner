rm ../lib/Mac/miner.Mac.*

make clean
make -f Makefile.dylib.Mac.debug
mv output/miner.Mac.debug.dylib ../lib/Mac

make clean
make -f Makefile.dylib.Mac.release
mv output/miner.Mac.release.dylib ../lib/Mac

