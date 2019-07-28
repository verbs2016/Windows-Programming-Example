// WinsockBlockingTcpServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

const int g_buff_len = 1024;
const int g_ipv4_len = 16;
const u_short port = 1111;

BOOL InitWinsockLib();
void DeInitWinsockLib();

int _tmain(int argc, _TCHAR* argv[])
{
	int error_code = -1;
	SOCKET server_socket = INVALID_SOCKET;
	SOCKADDR_IN server_addr = { 0 };
	SOCKET client_socket = INVALID_SOCKET;
	SOCKADDR_IN client_addr = { 0 };
	char ipv4_str[g_ipv4_len] = { 0 };
	char buff[g_buff_len] = { 0 };

	if (FALSE == InitWinsockLib())
		goto Cleanup;

	if (INVALID_SOCKET == (server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))){
		printf("%s: socket failed with error = %d\n", __FUNCTION__, WSAGetLastError());
		goto Cleanup;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_LOOPBACK);
	if (SOCKET_ERROR == bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr))){
		printf("%s: bind failed with error = %d\n", __FUNCTION__, WSAGetLastError());
		goto Cleanup;
	}

	// 打印出客户端的地址
	if (NULL == inet_ntop(AF_INET, &server_addr.sin_addr, ipv4_str, g_ipv4_len)) {
		printf("%s: inet_ntop failed with error = %s\n", __FUNCTION__, WSAGetLastError());
		goto Cleanup;
	}
	printf("server listen ip = %s:%d\n", ipv4_str, ntohs(server_addr.sin_port));

	if (SOCKET_ERROR == listen(server_socket, SOMAXCONN)){
		printf("%s: listen failed with error = %d\n", __FUNCTION__, WSAGetLastError());
		goto Cleanup;
	}
	
	// 接受客户端的连接请求
	int client_addr_len = sizeof(client_addr);
	client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_addr_len);
	if (INVALID_SOCKET == client_socket) {
		printf("%s: accept failed with error = %d\n", __FUNCTION__, WSAGetLastError());
		goto Cleanup;
	}

	ZeroMemory(ipv4_str, g_ipv4_len);
	// 打印出客户端的地址
	if (NULL == inet_ntop(AF_INET, &client_addr.sin_addr, ipv4_str, g_ipv4_len)) {
		printf("%s: inet_ntop failed with error = %s\n", __FUNCTION__, WSAGetLastError());
		goto Cleanup;
	}
	printf("Accept a client ip = %s:%d\n", ipv4_str, ntohs(client_addr.sin_port));

	// 接收客户端发来的消息
	error_code = recv(client_socket, buff, g_buff_len, 0);
	if (SOCKET_ERROR == error_code) {
		printf("%s: recv failed with error = %d\n", __FUNCTION__, WSAGetLastError());
		goto Cleanup;
	}
	printf("recv from client messages: %s\n", buff);

	ZeroMemory(buff, g_buff_len);
	strcpy_s(buff, "hello client");
	// 将消息发回给客户端
	error_code = send(client_socket, buff, strlen(buff), 0);
	if (SOCKET_ERROR == error_code) {
		printf("%s: send failed with error = %d\n", __FUNCTION__, WSAGetLastError());
		goto Cleanup;
	}

	error_code = 0;
Cleanup:
	if (INVALID_SOCKET == server_socket){
		closesocket(server_socket);
		server_socket = INVALID_SOCKET;
	}

	if (INVALID_SOCKET == client_socket) {
		closesocket(client_socket);
		client_socket = INVALID_SOCKET;
	}

	DeInitWinsockLib();

	return 0;
}


BOOL InitWinsockLib() {
	WSAData wsa_data;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa_data)){
		printf("%s: WSAStartup failed with error = %d\n", __FUNCTION__, GetLastError());
		return FALSE;
	}

	return TRUE;
}


void DeInitWinsockLib() {
	WSACleanup();
}