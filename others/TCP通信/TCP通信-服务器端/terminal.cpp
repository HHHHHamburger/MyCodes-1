#include <stdio.h>
#include <winsock.h>
#pragma comment(lib,"Ws2_32")
#pragma warning(disable:4996)
#define MAXDATASIZE 100 
#define MYPORT 6667  /*�����û����Ӷ˿�*/ 
#define BACKLOG 10  /*���ٵȴ����ӿ���*/
bool onoff;
int mode;
bool cooling;
bool updown;
bool leftright;
int countdown;
int tempreture;

void resetall()
{
	mode = 0;
	onoff = false;
	cooling = true;
	updown = false;
	leftright = false;
	tempreture = 26;
	countdown = -1;
}

int main()
{
	resetall();
	int sockfd, new_fd;                                  /*�����׽���*/
	struct sockaddr_in my_addr;          /*���ص�ַ��Ϣ */
	struct sockaddr_in their_addr;        /*�����ߵ�ַ��Ϣ*/
	int sin_size, numbytes;   char msg[10], buf[MAXDATASIZE];
	WSADATA ws;
	WSAStartup(MAKEWORD(2, 2), &ws);           //��ʼ��Windows Socket Dll
	//����socket
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		//�������socketʧ�ܣ��˳�����
		printf("socket error\n");
		exit(1);
	}
	bool optval = true;
	setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(bool));
	//bind������MYPORT�˿�
	my_addr.sin_family = AF_INET;                     /* Э��������INET  */
	my_addr.sin_port = htons(MYPORT);            /* ��MYPORT�˿�*/
	my_addr.sin_addr.s_addr = INADDR_ANY;    /* ����IP*/
	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
	{
		//bindʧ�ܣ��˳�����
		printf("bind error\n");
		closesocket(sockfd);
		exit(1);
	}


	//listen�������˿�
	printf("listen...\n");
	int ret;
	int size = sizeof(struct sockaddr);
	while (1)
	{
	//	if ((numbytes = recv(new_fd, buf, MAXDATASIZE, 0)) == -1)  continue;
		ret = recvfrom(sockfd, buf, MAXDATASIZE, 0, (SOCKADDR*)&my_addr, &size);
		if (!strcmp(buf, "bye"))
		{
			//�ɹ����ر��׽���

			closesocket(sockfd);
			//closesocket(new_fd);
			return 0;
		}
		printf("%s %d", buf, strlen(buf));
		printf("\n");
	}
}