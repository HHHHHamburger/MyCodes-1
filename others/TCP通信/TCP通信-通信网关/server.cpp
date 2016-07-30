#include <stdio.h>
#include <winsock.h>
#include <string.h>
#pragma comment(lib,"Ws2_32")
#pragma warning(disable:4996)
#define MAXDATASIZE 100 
#define MYPORT 6666  /*�����û����Ӷ˿�*/ 
#define SENDPORT 6667
#define BACKLOG 10  /*���ٵȴ����ӿ���*/ 
int main()
{
	int sockfd, new_fd,socktt;                                  /*�����׽���*/
	struct sockaddr_in my_addr;          /*���ص�ַ��Ϣ */
	struct sockaddr_in their_addr;        /*�����ߵ�ַ��Ϣ*/
	struct sockaddr_in terminal_addr;
	int sin_size, numbytes;   
	char msg[10], buf[MAXDATASIZE];
	char *PATHS = "255.255.255.255";

	
	WSADATA ws;
	WSAStartup(MAKEWORD(2, 2), &ws);           //��ʼ��Windows Socket Dll
	//����socket
	if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
	{
		//�������socketʧ�ܣ��˳�����
		printf("socket error\n");
		getchar();
		exit(1);
	}

	if ((socktt = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		//�������socketʧ�ܣ��˳�����
		printf("socket error\n");
		getchar();
		exit(1);
	}
	bool bbroadcast = true;
	setsockopt(socktt, SOL_SOCKET, SO_BROADCAST, (char*)&bbroadcast, sizeof(bool));

	//bind������MYPORT�˿�
	my_addr.sin_family = AF_INET;                     /* Э��������INET  */
	my_addr.sin_port = htons(MYPORT);            /* ��MYPORT�˿�*/
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);   /* ����IP*/

	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
	{
		//bindʧ�ܣ��˳�����
		printf("bind error\n");
		closesocket(sockfd);
		getchar();
		exit(1);
	}


	terminal_addr.sin_family = AF_INET;
	terminal_addr.sin_port = htons(SENDPORT);
	terminal_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	if (bind(socktt, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
	{
		//bindʧ�ܣ��˳�����
		printf("terminal bind error\n");
		closesocket(sockfd);
		getchar();
		exit(1);
	}
     
	
	//listen�������˿�
	if (listen(sockfd, BACKLOG) == -1)
	{
		//listenʧ�ܣ��˳�����
		printf("listen error\n");
		closesocket(sockfd);
		getchar();
		exit(1);
	}
	printf("listen...\n");
	//�ȴ��ͻ�������
	sin_size = sizeof(struct sockaddr_in);

	if ((new_fd = accept(sockfd, (sockaddr*)&their_addr, &sin_size)) == -1)
	{
		printf("accept error\n");
		closesocket(sockfd);
		getchar();
		exit(1);
	}

	while (1) 
	{
		if ((numbytes = recv(new_fd, buf, MAXDATASIZE, 0)) == -1)  continue;

		printf("%s", buf);
		printf("\n");
		sprintf(msg, "%d", strlen(buf));

		if (send(new_fd, msg, MAXDATASIZE, 0) == -1)
		{
			printf("send ERROR.\n");
			closesocket(sockfd);
			closesocket(new_fd);
			getchar();
			return 0;
		}


		if (sendto(socktt, buf, MAXDATASIZE, 0, (SOCKADDR*)&terminal_addr, sizeof(SOCKADDR)) == -1)
		{
			printf("send error.\n");
			closesocket(sockfd);
			GetLastError();
			getchar();
			exit(1);
		}
		else
			printf("send succeed.\n");

		if (!strcmp(buf, "bye"))
		{
			//�ɹ����ر��׽���

			closesocket(sockfd);
			closesocket(new_fd);
			closesocket(socktt);
			getchar();
			return 0;
		}
	}

}