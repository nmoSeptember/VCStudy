// socket.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib,"ws2_32")

int main(int argc, char *argv[])
{
	struct in_addr ss;
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	SOCKET s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in sockaddr;
	sockaddr.sin_family = PF_INET;
	sockaddr.sin_addr.S_un.S_addr = inet_pton(PF_INET, "127.0.0.1", (void*)&ss);
	sockaddr.sin_port = htons(827);
	bind(s, (SOCKADDR*)&sockaddr, sizeof(SOCKADDR));
	listen(s, 1);
	SOCKADDR clientAddr;
	int nSize = sizeof(SOCKADDR);
	SOCKET clientSock;
	clientSock = accept(s, (SOCKADDR*)&clientAddr, &nSize);
	send(clientSock, "Hello Client\r\n", strlen("Hello Client\r\n") + sizeof(char), NULL);
	closesocket(clientSock);
	closesocket(s);
	WSACleanup();
	return 0;
}
