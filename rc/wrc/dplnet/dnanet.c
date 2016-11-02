#include <Windows.h>
#include <stdio.h>
#include "dnanet.h"

#define BUFSIZE 512

#pragma comment(lib, "ws2_32.lib")

void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s]%s", msg, (LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}
int dnaOpen(char* ip, int port, int type)
{
	int e = -1;
	WSADATA wsa;
	int retval;

	if (type & 0x80 == 0x80)  /// UDP
	{
		if (type & 0x08 == 0x80) /// CLIENT
		{
			SOCKET ucs = socket(PF_INET, SOCK_DGRAM, 0);

		}
		else  /// SERVER
		{
			SOCKET uss = socket(PF_INET, SOCK_DGRAM, 0);
		}
	}
	else /// TCP
	{
		if (type & 0x08 == 0x80) /// CLIENT
		{
			struct sockaddr_in server_addr;
			
			SOCKET tcs = socket(PF_INET, SOCK_STREAM, 0);

			ZeroMemory(&server_addr, sizeof(server_addr));
			server_addr.sin_family = AF_INET;
			server_addr.sin_port = htons(port);
			server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		}
		else  /// SERVER
		{
			printf("TCP server\r\n");

			if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) 
			{
				return -1;
			}
			SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);

			SOCKADDR_IN serveraddr;
			ZeroMemory(&serveraddr, sizeof(serveraddr));
			serveraddr.sin_family = AF_INET;
			serveraddr.sin_port = htons(port);
			serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
			retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
			retval = listen(listen_sock, SOMAXCONN);

			return listen_sock;
		}
	}
	return e;
}

int dnaClose(int sd)
{
	int e = -1;

	closesocket(sd);

	return e;
}

int dnaAccept(int sd, char* ip, int sz)
{
	int e = -1;
	SOCKADDR_IN clientaddr;
	int addrlen , retval;
	char buf[BUFSIZE + 1];

	addrlen = sizeof(clientaddr);
	e = accept(sd, (SOCKADDR*)&clientaddr, &addrlen);

	if (e == INVALID_SOCKET) 
	{
		err_display("accept()");
	}
	printf("\n[TCP 서버] 클라이언트 접속 : IP주소=%s, 포트번호=%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	return e;	
}


int dnaRead(int sd, char* buf, int sz, char* ip, int port)
{
	int e = -1;
	struct sockaddr_in addr;
	int _sz = 0;
	if (ip == 0)
	{
		e = recv(sd, buf, sz, 0);
	}
	else
	{
		e = recvfrom(sd, buf, sz, 0, (sockaddr*)&addr, &_sz);
	}
	return e;
}

int dnaWrite(int sd, char* buf, int sz, char* ip, int port)
{
	int e = -1;

	if (ip == 0)
	{
		e = send(sd, buf, sz, 0);
	}
	else
	{
		e = sendto(sd, buf, sz, 0, 0, sizeof(struct sockaddr));
	}

	return e;
}
