// WinsockBlockingTcpClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

const int g_buff_len = 1024;
const u_short port = 1111;

BOOL InitWinsockLib();
void DeInitWinsockLib();


int _tmain(int argc, _TCHAR* argv[]) {
	int error_code = -1;
	SOCKET client_socket = INVALID_SOCKET;
	SOCKADDR_IN server_addr = { 0 };
	char buff[g_buff_len] = { 0 };

	if (FALSE == InitWinsockLib())
		goto Cleanup;

	// ������ socket Ĭ�����Զ���������
	if (INVALID_SOCKET == (client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))) {
		printf("%s: socket failed with error = %d\n", __FUNCTION__, WSAGetLastError());
		goto Cleanup;
	}
	
	// ׼�����ӷ������ĵ�ַ��������Ҫע��һ�µ�ַ�������ֽ���
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_LOOPBACK);
	server_addr.sin_port = htons(port);
	if (SOCKET_ERROR == connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr))) {
		printf("%s: connect failed with error = %d\n", __FUNCTION__, WSAGetLastError());
		goto Cleanup;
	}

	// ׼��һ������/���յĻ���������Ϊ���������ر�򵥣�ֱ���жϷ���ֵ
	strcpy_s(buff, "hello server");
	error_code = send(client_socket, buff, strlen(buff), 0);
	if (error_code == SOCKET_ERROR) {
		printf("%s: send failed with error = %d\n", __FUNCTION__, WSAGetLastError());
		goto Cleanup;
	}

	ZeroMemory(buff, g_buff_len);
	error_code = recv(client_socket, buff, g_buff_len, 0);
	if (error_code == SOCKET_ERROR) {
		printf("%s: recv failed with error = %d\n", __FUNCTION__, WSAGetLastError());
		goto Cleanup;
	} else {
		printf("recv from server messages: %s\n", buff);
	}
	
	error_code = 0;
Cleanup:

	if (INVALID_SOCKET != client_socket){
		closesocket(client_socket);
		client_socket = INVALID_SOCKET;
	}

	// ��������������ʵû��Ҫ��װ��������Ϊ�˶Գ�
	DeInitWinsockLib();

	return error_code;
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