#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <winsock2.h>
#include<iostream>
#define MAX_MSG_SIZE 1024
#define SERVER_PORT 49152
#define BACKLOG 5
#pragma comment(lib, "Ws2_32.lib")
using namespace std;
int main()
{
	printf("Lab6 UDP-������\n");
	WORD version(0);
	WSADATA wsadata;
	int socket_return(0);
	version = MAKEWORD(2, 0);
	socket_return = WSAStartup(version, &wsadata);//��ʼ���׽��ֿ��ʹ�á�
	if (socket_return != 0)
	{
        getchar(); getchar();
		return 0;
	}//��Щ�����٣�����ٵĻ������޷���ȷ����socket��

	//ɾ��data.txt�ļ�
	remove("data.txt");

	int ser_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);/*�������ӵ�SOCKET */
	
	timeval tv = { 100, 0 };
	setsockopt(ser_sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(timeval));

	struct sockaddr_in ser_addr; /* server��ַ��Ϣ */
	struct sockaddr_in cli_addr; /* �ͻ��˵�ַ��Ϣ */
	struct sockaddr_in sen_addr; /* �ͻ��˵�ַ��Ϣ */

	char msg[MAX_MSG_SIZE];/* ������,��Ŵӿͻ�����������*/
	memset(msg, 0, MAX_MSG_SIZE);//���

	if (ser_sockfd < 0)
	{/*����ʧ�� */
		fprintf(stderr, "socker Error:%s\n", strerror(errno));
        getchar(); getchar();
		exit(1);
	}
	/* ��ʼ����������ַ*/
	int addrlen = sizeof(struct sockaddr_in);
	int cli_addrlen = sizeof(struct sockaddr_in);
	int sen_addrlen = sizeof(struct sockaddr_in);
	memset(&ser_addr, 0, addrlen);
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);//INADDR_ANY ���Ǳ���������ip
//�ֶ�ָ��ip
// my_addr.sin_addr.s_addr   =   inet_addr("xxx.xxx.xxx.xxx"); inet_addr���ڰ�32λ��ip��ַתΪunsigned long  

	ser_addr.sin_port = htons(SERVER_PORT);//Ϊ���server����ָ���˿ں�
	if (bind(ser_sockfd, (struct sockaddr*)&ser_addr, sizeof(struct sockaddr_in))
		< 0)
	{ /*��ʧ�� */
		fprintf(stderr, "Bind Error:%s\n", strerror(errno));
		exit(1);
	}
	printf("����˴򿪳ɹ����ȴ����ӿͻ���...\n");
	/*int cli_sockfd = accept(ser_sockfd, (struct sockaddr*)&cli_addr, &addrlen);*/
	int num = 0;
	while (1)
	{/* �ȴ����տͻ���������*/

		memset(msg, 0, MAX_MSG_SIZE);

			/*memset(msg, 0, MAX_MSG_SIZE);*/
			int len = recvfrom(ser_sockfd, msg, MAX_MSG_SIZE, 0,(struct sockaddr*)&cli_addr,&cli_addrlen);
			if (len <= 0) {

				num++;
				if(num>100){
				cerr << "�ͻ��� IP��127.0.0.1 ������ʧȥ���ӡ�" << endl;
				remove("data.txt");
                getchar(); getchar();
				exit(-1);
                }
				else {
					continue;
				}
			}
			num = 0;
			printf("���յ��ͻ����������� from %s\n", inet_ntoa(cli_addr.sin_addr));
			printf("�յ��ͻ��˷��͵����ݣ�\n");
			printf("%s\n", msg);/*����Ļ�ϴ�ӡ���� */

			FILE* fp;
			fp = fopen("data.txt", "a+");
			if (fp == NULL)
			{
				printf("File Can Not Open To Write!\n");
				_exit(-1);
			}
			printf("�����յ�������д���ļ���...\n");
			//����fwrite������recv_buf�����е�����д���ļ���
			int write_length = fwrite(msg, sizeof(char), len, fp);
			if (write_length < len)
			{
				printf("�ļ�д��ʧ��!\n");
				break;
			}
			printf("���տͻ��˴�����������ɹ�!\n");
			fclose(fp);


			/*���͵�����*/

			sendto(ser_sockfd, "���������յ��ļ�!", sizeof(msg), 0,(struct sockaddr*)&cli_addr,cli_addrlen);
			//closesocket(cli_sockfd);
		//}
	}//server��infinity������������while(1)

	closesocket(ser_sockfd);
}

