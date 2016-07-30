#include <stdio.h>
#include <winsock.h>
#pragma comment(lib,"Ws2_32")
#pragma warning(disable:4996)
#define SERVER_PORT 6666 //�����˿�
#define SEND_PORT 6667
int main()
{
	char addr[20];
	printf("Please enter address:");
	scanf("%s",addr);
	WORD wVersionRequested;
	WSADATA wsaData;
	int ret, nLeft, length;
	SOCKET sListen, sServer,sClient; //�����׽��֣������׽���
	struct sockaddr_in saServer, saClient,saSend; //��ַ��Ϣ   
	char *ptr;//���ڱ�����Ϣ��ָ��   
	//WinSock��ʼ��
	wVersionRequested = MAKEWORD(2, 2); //ϣ��ʹ�õ�WinSock DLL �İ汾
	ret = WSAStartup(wVersionRequested, &wsaData);
	if (ret != 0)
	{
		printf("WSAStartup() failed!\n");
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
	if (sListen == INVALID_SOCKET)
	{
		WSACleanup();
		printf("socket() faild!\n");
		return -1;
	}
	//�������ص�ַ��Ϣ
	saServer.sin_family = AF_INET; //��ַ����
	saServer.sin_port = htons(SERVER_PORT); //ע��ת��Ϊ�����ֽ���
	saServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY); //ʹ��INADDR_ANY ָʾ�����ַ

	saSend.sin_family = AF_INET; //��ַ����
	saSend.sin_port = htons(SEND_PORT); //ע��ת��Ϊ�������
	saSend.sin_addr.S_un.S_addr = inet_addr(addr);


	//��
	ret = bind(sListen, (struct sockaddr *)&saServer, sizeof(saServer));
	if (ret == SOCKET_ERROR)
	{
		printf("bind() faild! code:%d\n", WSAGetLastError());
		closesocket(sListen); //�ر��׽���
		WSACleanup();
		return -1;
	}

	//������������
	ret = listen(sListen, 5);
	if (ret == SOCKET_ERROR)
	{
		printf("listen() faild! code:%d\n", WSAGetLastError());
		closesocket(sListen); //�ر��׽���
		return -1;
	}

	printf("Waiting for client connecting!\n");
	printf("Tips: Ctrl+c to quit!\n");
	length = sizeof(saClient);
	sServer = accept(sListen, (struct sockaddr *)&saClient, &length);
	if (sServer == INVALID_SOCKET)
	{
		printf("accept() faild! code:%d\n", WSAGetLastError());
		closesocket(sListen); //�ر��׽���
		WSACleanup();
		return -1;
	}



	ret = connect(sClient, (struct sockaddr *)&saSend, sizeof(saClient));
	if (ret == SOCKET_ERROR)
	{
		printf("connect() failed!\n");
		closesocket(sClient); //�ر��׽���
		WSACleanup();
		return -1;
	}

	//�����ȴ����ܿͻ�������
	while (1)//ѭ�������ͻ��ˣ���Զ��ֹͣ�����ԣ��ڱ���Ŀ�У�����û����������
	{


		char receiveMessage[255];
			//��������
		ret = recv(sServer,receiveMessage, sizeof(receiveMessage), 0);
		if (ret == SOCKET_ERROR)
		{
			printf("recv() failed!\n");
			return -1;
		}
		printf("%s\n", receiveMessage);
		gets(receiveMessage);
		ret = send(sClient,receiveMessage, sizeof(receiveMessage), 0);
		if(ret==SOCKET_ERROR)
			printf("send error");
	}
	closesocket(sListen);
	closesocket(sServer);
	WSACleanup();
	return 0;
}