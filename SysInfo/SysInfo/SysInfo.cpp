// SysInfo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <Windows.h>

int main()
{
	TCHAR lpStrBuf[MAX_PATH];
	unsigned long uSize = MAXBYTE;

	if (!GetComputerName(lpStrBuf, &uSize))
		std::cout << "GetComputerName Error:" << GetLastError() << std::endl;
	else
#ifdef _UNICODE
		std::wcout << lpStrBuf << std::endl;
#endif // _UNICODE

		
	std::cout << std::endl;
}

