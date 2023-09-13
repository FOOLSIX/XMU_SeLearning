#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <winsock2.h>
#include <iostream>
#define MAX_MSG_SIZE 1024
#define SERVER_PORT 49152
#define BACKLOG 5
#pragma comment(lib, "Ws2_32.lib")
using namespace std;
int main()
{
	printf("Lab6 TCP-服务器\n");
	WORD version(0);
	WSADATA wsadata;
	int socket_return(0);
	version = MAKEWORD(2, 0);
	socket_return = WSAStartup(version, &wsadata);//开始对套接字库的使用。
	if (socket_return != 0)
	{
        getchar(); getchar();
		return 0;
	}//这些不能少，如果少的话，就无法正确创建socket。

	//删除data.txt文件
	remove("data.txt");

	int ser_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);/*创建连接的SOCKET */


	struct sockaddr_in ser_addr; /* server地址信息 */
	struct sockaddr_in cli_addr; /* 客户端地址信息 */
	char msg[MAX_MSG_SIZE];/* 缓冲区,存放从客户端来的请求*/
	memset(msg, 0, MAX_MSG_SIZE);//清空

	if (ser_sockfd < 0)
	{/*创建失败 */
		fprintf(stderr, "socker Error:%s\n", strerror(errno));
        getchar(); getchar();
		exit(1);
	}
	/* 初始化服务器地址*/
	int addrlen = sizeof(struct sockaddr_in);
	memset(&ser_addr, 0, addrlen);
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);//INADDR_ANY 就是本机的所有ip
//如果要手动指定ip可以这么使用
// my_addr.sin_addr.s_addr   =   inet_addr("xxx.xxx.xxx.xxx"); inet_addr用于把32位的ip地址转为unsigned long  

	ser_addr.sin_port = htons(SERVER_PORT);//为这个server进程指定端口号
	if (bind(ser_sockfd, (struct sockaddr*)&ser_addr, sizeof(struct sockaddr_in))
		< 0)
	{ /*绑定失败 */
		fprintf(stderr, "Bind Error:%s\n", strerror(errno));
        getchar(); getchar();
		exit(1);
	}
	printf("服务端打开成功，等待连接客户端...\n");
	/*侦听客户端请求*/
	if (listen(ser_sockfd, BACKLOG) < 0)//BACKLOG的作用就是设置accpet接收请求队列的大小
	{
		fprintf(stderr, "Listen Error:%s\n", strerror(errno));
		closesocket(ser_sockfd);
        getchar(); getchar();
		exit(1);
	}
	int cli_sockfd = accept(ser_sockfd, (struct sockaddr*)&cli_addr, &addrlen);
	while (1)
	{/* 等待接收客户连接请求*/

		//int cli_sockfd = accept(ser_sockfd, (struct sockaddr*)&cli_addr,&addrlen);//传入绑定好的进程插口，和客户端地址（ip+port，给出的是cli_addr，以及它的长度）实现通信
	//	printf("%d",cli_sockfd);
		if (cli_sockfd <= 0)
		{
			fprintf(stderr, "Accept Error:%s\n", strerror(errno));
		}
		else
		{/*开始服务*/

		/* 接受数据*/
			memset(msg, 0, MAX_MSG_SIZE);
			int i=recv(cli_sockfd, msg, MAX_MSG_SIZE, 0);
			if (i <=0) {
				cerr << "客户端 IP：127.0.0.1 过程中失去连接。" << endl;
				remove("data.txt");
                getchar(); getchar();
				exit(-1);
			}
			printf("接收到客户端连接请求 from %s\n", inet_ntoa(cli_addr.sin_addr));
			printf("收到客户端发送的数据：\n");
			printf("%s\n", msg);/*在屏幕上打印出来 */
			
			FILE* fp;
			fp = fopen("data.txt", "a+");
			if (fp == NULL)
			{
				printf("File Can Not Open To Write!\n");
				_exit(-1);
			}
			printf("将接收到的数据写入文件中...\n");
			//调用fwrite函数将recv_buf缓存中的数据写入文件中
			int write_length = fwrite(msg, sizeof(char), i, fp);
			if (write_length < i)
			{
				printf("文件写入失败!\n");
				break;
			}
			printf("接收客户端传输的数据流成功!\n");
			fclose(fp);


			/*发送的数据*/
			
			send(cli_sockfd, "服务器已收到文件!", sizeof(msg), 0);
			//closesocket(cli_sockfd);
		}
	}//server是infinity工作，所以是while(1)
	closesocket(cli_sockfd);
	closesocket(ser_sockfd);
}
