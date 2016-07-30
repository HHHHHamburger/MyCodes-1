#include <stdio.h>
#include <stdlib.h>
#include <winsock.h>
#pragma comment(lib,"Ws2_32")
#pragma warning(disable:4996)
#define SERVER_PORT 6666 //�����˿�
#define LISTEN_PORT 6667
int main()
{
	char addr[20];
	printf("Please enter address:");
	scanf("%s",addr);
	WORD wVersionRequested;
	WSADATA wsaData;
	int ret;
	SOCKET sClient,sListen, sServer; //�����׽���
	struct sockaddr_in saServer,saClient; //��ַ��Ϣ
	char *ptr;
	BOOL fSuccess = TRUE;
	//WinSock��ʼ��
	wVersionRequested = MAKEWORD(2, 2); //ϣ��ʹ�õ�WinSock DLL�İ汾
	ret = WSAStartup(wVersionRequested, &wsaData);
	if (ret != 0)
	{
		printf("WSAStartup() failed!\n");
		return -1;
	}
	//ȷ��WinSock DLL֧�ְ汾2.2
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		printf("Invalid WinSock version!\n");
		return -1;
	}
	//����Socket,ʹ��TCPЭ��
	sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sClient == INVALID_SOCKET)
	{
		WSACleanup();
		printf("socket() failed!\n");
		return -1;
	}
	//������������ַ��Ϣ
	saServer.sin_family = AF_INET; //��ַ����
	saServer.sin_port = htons(SERVER_PORT); //ע��ת��Ϊ�������
	saServer.sin_addr.S_un.S_addr = inet_addr(addr);

	saClient.sin_family = AF_INET; //��ַ����
	saClient.sin_port = htons(LISTEN_PORT); //ע��ת��Ϊ�����ֽ���
	saClient.sin_addr.S_un.S_addr = htonl(INADDR_ANY); //ʹ��INADDR_ANY ָʾ�����ַ


	//���ӷ�����
	ret = connect(sClient, (struct sockaddr *)&saServer, sizeof(saServer));
	ret = bind(sListen, (struct sockaddr *)&saClient, sizeof(saClient));
	if (ret == SOCKET_ERROR)
	{
		printf("connect() failed!\n");
		closesocket(sClient); //�ر��׽���
		WSACleanup();
		return -1;
	}
	if (ret == SOCKET_ERROR)
	{
		printf("bind() faild! code:%d\n", WSAGetLastError());
		closesocket(sListen); //�ر��׽���
		WSACleanup();
		getchar();
				getchar();
		return -1;
	}
	ret = listen(sListen, 5);
	int length=sizeof(saClient);
	sServer = accept(sListen, (struct sockaddr *)&saClient, &length);

	char buf[255];
	do
	{
		gets(buf);
		ret = send(sClient, (char *)&buf, sizeof(buf), 0);
		if (ret == SOCKET_ERROR)
		{
			printf("send() failed!\n");
		}
		ret = recv(sServer,buf, sizeof(buf), 0);
		printf("%s\n",buf);
	} while (ret != SOCKET_ERROR);
	closesocket(sClient); //�ر��׽���
	WSACleanup();
}