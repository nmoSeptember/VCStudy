// TrojanClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib,"ws2_32")

int main()
{
	struct in_addr sAdd;
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	SOCKET clientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in serverAddr;
	serverAddr.sin_family = PF_INET;
	inet_pton(PF_INET, "127.0.0.1", &sAdd);
	serverAddr.sin_addr.S_un.S_addr = sAdd.S_un.S_addr;
	serverAddr.sin_port = htons(182766);
	connect(clientSocket, (SOCKADDR*)& serverAddr, sizeof(SOCKADDR));
	while (true)
	{
		char buff[MAXBYTE] = { 0 };
		char cmd[MAXBYTE] = { 0 };

		recv(clientSocket, buff, MAXBYTE, 0);
		std::cout << buff << std::endl;
		std::cin >> cmd;
		send(clientSocket, cmd, MAXBYTE, 0);
		if (!strcmp(cmd,"exit"))
		{
			std::cout << "Login out!\r\n";
			break;
		}
		memset(buff, 0, MAXBYTE);
	}
}
