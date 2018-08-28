#!bin/sh

./test 50 150 4 > zcash_rds_50_150_4.txt
./test 50 150 5 > zcash_rds_50_150_5.txt
./test 50 150 6 > zcash_rds_50_150_6.txt
./test 50 100 4 > zcash_rds_50_100_4.txt

./test 150 250 4 > zcash_inc_150_250_4.txt
./test 150 250 5 > zcash_inc_150_250_5.txt
./test 150 250 6 > zcash_inc_150_250_6.txt
./test 200 400 4 > zcash_inc_200_400_4.txt


./test 90 210 4 > zcash_inc_90_210_4.txt

