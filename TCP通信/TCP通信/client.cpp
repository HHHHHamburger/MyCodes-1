#include <stdio.h>
#include <stdio.h>
#include <winsock.h>
#pragma comment(lib,"Ws2_32")
#pragma warning(disable:4996)
#define PORT 6666                         /* �ͻ�������Զ�������Ķ˿� */ 
#define MAXDATASIZE 100                     /* ÿ�ο��Խ��յ�����ֽ� */ 
int main()
{
	int sockfd, numbytes;
	char buf[MAXDATASIZE];
	char msg[MAXDATASIZE];
	char *argv = "111.117.104.77";
	struct sockaddr_in their_addr;        /* �Է��ĵ�ַ�˿���Ϣ */

	WSADATA ws; 
	WSAStartup(MAKEWORD(2, 2), &ws);         //��ʼ��Windows Socket Dll
	if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
	{
		//�������socketʧ�ܣ��˳�����
		printf("socket error\n");
		exit(1);
	}

	//���ӶԷ�
	their_addr.sin_family = AF_INET;                         /* Э��������INET  */
	their_addr.sin_port = htons(PORT);                       /* ���ӶԷ�PORT�˿� */
	their_addr.sin_addr.s_addr = inet_addr(argv);        /* ���ӶԷ���IP */
	if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1)
	{
		//�������ʧ�ܣ��˳�����
		printf("connet error\n");
		closesocket(sockfd);
		exit(1);
	}
	printf("connet succeed\n");
	//strcpy(msg,"hellodavid");
	while (1){
		scanf("%s", msg);
		//��������
			if (send(sockfd,msg,MAXDATASIZE,0) == -1)
			{
				printf("send error");
				closesocket(sockfd);
				getchar();
				exit(1);
			}

			//�������ݣ�����ӡ����
			if ((numbytes = recv(sockfd, buf, MAXDATASIZE, 0)) == -1)
			{
				//��������ʧ�ܣ��˳�����
				printf("recv error\n");
				closesocket(sockfd);
				getchar();
				exit(1);
			}
			//buf[numbytes] = '\0';
			printf("received: %s\n", buf);
	}
	closesocket(sockfd);
	return 0;
}