// socketClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include<WINSOCK2.H>
#include<STDIO.H>
#include<iostream>
#include<cstring>
#include <WS2tcpip.h>

using namespace std;
#pragma comment(lib, "ws2_32.lib") 
int main() {
	WORD sockVersion = MAKEWORD(2, 2);	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0) { return 0; }	while (true) {
		SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);		if (sclient == INVALID_SOCKET)
		{
			printf("invalid socket!");			return 0;
		}
		sockaddr_in serAddr;		serAddr.sin_family = AF_INET;
		serAddr.sin_port = htons(8888);
		char buff[MAXBYTE];
		serAddr.sin_addr.S_un.S_addr = inet_pton(PF_INET, "127.0.0.1", buff);

		if (connect(sclient, (sockaddr*)& serAddr, sizeof(serAddr)) == SOCKET_ERROR) {  //����ʧ��
			printf("connect error !");			closesocket(sclient);			return 0;
		}
		string data;		cin >> data;		const char* sendData;		sendData = data.c_str();   //stringתconst char
		send(sclient, "��ã�TCP����ˣ����ǿͻ���\n", strlen("��ã�TCP����ˣ����ǿͻ���\n"), 0);
		//send()������������ָ����socket�����Է�����		//
		int send(int s, const void* msg, int len, unsigned int flags);
		//sΪ�ѽ��������ӵ�socket��msgָ���������ݣ�len��Ϊ���ݳ��ȣ�����flagsһ����0		
		//�ɹ��򷵻�ʵ�ʴ��ͳ�ȥ���ַ�����ʧ�ܷ���-1������ԭ�����error 				
		char recData[255];
		int ret = recv(sclient, recData, 255, 0);
		if (ret > 0) { recData[ret] = 0x00;			printf(recData); }
		closesocket(sclient);
	}			WSACleanup();	return 0;
}

