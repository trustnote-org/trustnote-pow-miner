//
// Created by Liu QiXing on 2018/10/23.
//
#include <iostream>
using namespace std;


bool isLittleEndian();
bool isBigEndian();



int main()
{
	printf( "isLittleEndian\t: %s\n", isLittleEndian() ? "yes" : "no" );
	printf( "isBigEndian\t: %s\n", isBigEndian() ? "yes" : "no" );
}


bool isLittleEndian()
{
	int a	= 0x1234;
	char b	= *(char *)&a;
	return ( 0x34 == b );
}

bool isBigEndian()
{
	int a	= 0x1234;
	char b	= *(char *)&a;
	return ( 0x12 == b );
}