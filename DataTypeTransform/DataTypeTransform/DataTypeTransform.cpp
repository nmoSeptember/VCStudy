// DataTypeTransform.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <stdlib.h>

int main()
{
	int iType = 8;
	char szChar[_MAX_U64TOSTR_BASE2_COUNT];
	_itoa_s(iType, szChar, _countof(szChar), 2);
	std::cout << szChar << std::endl;

	int iTemp = atoi(szChar);
	std::cout << iTemp << std::endl;
}