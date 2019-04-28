// ping.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib,"ws2_32")

struct icmp_header
{
	unsigned char icmp_type;
	unsigned char icmp_code;
	unsigned short icmp_checksum;
	unsigned short icmp_id;
	unsigned short icmp_sequence;
	//unsigned long icmp_timestamp;
};

#define ICMP_HEADER_SIZE sizeof(icmp_header)
#define ICMP_ECHO_REQUEST 0x08
#define ICMP_ECHO_REPLY 0x00

unsigned short chsum(struct icmp_header* picmp, int len)
{
	long sum = 0;
	unsigned short* pusicmp = (unsigned short*)picmp;
	while (len > 1)
	{
		sum += *(pusicmp++);
		if (sum & 0x80000000)
		{
			sum = (sum & 0xffff) + (sum >> 16);
		}
		len -= 2;
	}
	if (len)
	{
		sum += (unsigned short) * (unsigned char*)pusicmp;
	}
	while (sum >> 16)
	{
		sum = (sum & 0xffff) + (sum >> 16);
	}
	return (unsigned short)~sum;
}

BOOL MyPing(char* szDestIP)
{
	BOOL bRet = true;
	WSADATA wsaData;
	int nTimeOut = 1000;
	char szBuff[ICMP_HEADER_SIZE + 32] = { 0 };
	icmp_header* pIcmp = (icmp_header*)szBuff;
	char icmp_data[32] = { 0 };
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		std::cout << "WSAStartup failed!" << std::endl;
	}
	SOCKET s = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char const*)& nTimeOut, sizeof(nTimeOut));

	struct in_addr addrBuff;
	sockaddr_in dest_addr;
	dest_addr.sin_family = PF_INET;
	inet_pton(PF_INET, szDestIP, &addrBuff);
	dest_addr.sin_addr.S_un.S_addr = addrBuff.S_un.S_addr;
	dest_addr.sin_port = htons(0);

	pIcmp->icmp_type = ICMP_ECHO_REQUEST;
	pIcmp->icmp_code = 0;
	pIcmp->icmp_id = (USHORT)::GetCurrentProcessId();
	pIcmp->icmp_sequence = 0;
	//pIcmp->icmp_timestamp = 0;
	pIcmp->icmp_checksum = 0;

	memcpy((szBuff + ICMP_HEADER_SIZE), "abcdefghijklmnopqrstuvwabcdefghi", 32);

	pIcmp->icmp_checksum = chsum((struct icmp_header*)szBuff, sizeof(szBuff));

	sockaddr_in from_addr;
	char szRecvBuff[MAXBYTE];
	int nLen = sizeof(from_addr);
	sendto(s, szBuff, sizeof(szBuff), 0, (SOCKADDR*)& dest_addr, sizeof(SOCKADDR));
	recvfrom(s, szRecvBuff, MAXBYTE, 0, (SOCKADDR*)& from_addr, &nLen);

	char sourceIp[MAXBYTE];
	char* ptr = const_cast<char*>(inet_ntop(PF_INET, &from_addr.sin_addr.S_un.S_addr, (PSTR)sourceIp, MAXBYTE));
	if (lstrcmpA(sourceIp, szDestIP))
	{
		bRet = FALSE;
	}
	else
	{
		char addrBuff2[MAXBYTE];
		inet_ntop(PF_INET, &from_addr.sin_addr.S_un.S_addr, (PSTR)addrBuff2, MAXBYTE);
		struct icmp_header* pIcmp1 = (icmp_header*)(szRecvBuff + 20);
		std::cout << addrBuff2 << std::endl;
	}
	return bRet;
}

int main()
{
	MyPing(const_cast<char*>("42.121.252.58"));
}