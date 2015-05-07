#include <stdio.h>
#include <stdlib.h>
#include <winsock.h>
#pragma comment(lib,"Ws2_32")
#pragma warning(disable:4996)
#define SERVER_PORT 6666 //�����˿�
void main()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int ret;
	SOCKET sClient; //�����׽���
	struct sockaddr_in saServer; //��ַ��Ϣ
	char *ptr;
	BOOL fSuccess = TRUE;
	//WinSock��ʼ��
	wVersionRequested = MAKEWORD(2, 2); //ϣ��ʹ�õ�WinSock DLL�İ汾
	ret = WSAStartup(wVersionRequested, &wsaData);
	if (ret != 0)
	{
		printf("WSAStartup() failed!\n");
		return;
	}
	//ȷ��WinSock DLL֧�ְ汾2.2
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		printf("Invalid WinSock version!\n");
		return;
	}
	//����Socket,ʹ��TCPЭ��
	sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sClient == INVALID_SOCKET)
	{
		WSACleanup();
		printf("socket() failed!\n");
		return;
	}
	//������������ַ��Ϣ
	saServer.sin_family = AF_INET; //��ַ����
	saServer.sin_port = htons(SERVER_PORT); //ע��ת��Ϊ�������
	saServer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	//���ӷ�����
	ret = connect(sClient, (struct sockaddr *)&saServer, sizeof(saServer));
	if (ret == SOCKET_ERROR)
	{
		printf("connect() failed!\n");
		closesocket(sClient); //�ر��׽���
		WSACleanup();
		return;
	}
	char buf[255];
	do
	{
		scanf("%s", buf);
		ret = send(sClient, (char *)&buf, sizeof(buf), 0);
		if (ret == SOCKET_ERROR)
		{
			printf("send() failed!\n");
		}
	} while (ret != SOCKET_ERROR);
	closesocket(sClient); //�ر��׽���
	WSACleanup();
}