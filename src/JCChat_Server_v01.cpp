#include <iostream>
#include <cstdio>
#include <string.h>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

//多线程
#include <pthread.h>

//tcp头文件
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <errno.h>
using namespace std;

const int BUF_SIZE = 1024;
const int MAX_CONNECT = 300;
int clnt_cnt;				 //已连接的客户端数量
int clnt_socks[MAX_CONNECT]; //客户端socket数组

char msg_buf[BUF_SIZE];

void error_handling(const char *msg);

pthread_mutex_t mutx;							   //互斥锁，保护clnt_socks数组
pthread_mutex_t mutx_clnt_sock;					   //互斥锁，保护clnt_sock
void * connect_handle(void *arg);				   //处理连接的线程函数
void broadcast_msg(const char msg[], int msgLen); //广播消息的函数

int main()
{
	//初始化互斥锁
	pthread_mutex_init(&mutx, NULL);
	pthread_mutex_init(&mutx_clnt_sock,NULL);

	// tcp基础设置
	int clnt_sock; //用于临时存储新连接的客户端socket
	struct sockaddr_in clnt_adr;
	socklen_t clnt_adr_size = sizeof(clnt_adr);

	int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket error");
	printf("socket success!\n");

	struct sockaddr_in serv_adr;
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr("10.0.20.9");
	serv_adr.sin_port = htons(8000);

	//设置允许端口复用
	int val = 1;
	setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void *)&val, sizeof(serv_sock));

	if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind error");
	printf("bind success!\n");

	if (listen(serv_sock, 5) == -1)
		error_handling("listen error");
	printf("start listening!\n");

	//开始接收客户端连接的循环
	while (1)
	{
		//保护clnt_sock，保证他先被复制到connect_handle内的线程局部变量
		pthread_mutex_lock(&mutx_clnt_sock);

		//接收新的连接
		clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_size);
		if (clnt_sock == -1)
		{
			printf("accept error!\n");
			continue;
		}
		printf("accept clnt ip: %s , socket: %d\n", inet_ntoa(clnt_adr.sin_addr), clnt_sock);

		//将要访问clnt_socks
		pthread_mutex_lock(&mutx);

		pthread_t thread_id;
		if (pthread_create(&thread_id, NULL, connect_handle, (void *)&clnt_sock) == 0)
			printf("create connect handle thread success! Thread id : %lu\n", thread_id);
		else
		{
			printf("create connect handle thread failed");
			continue;
		}

		clnt_socks[clnt_cnt] = clnt_sock;
		clnt_cnt++;

		pthread_mutex_unlock(&mutx);
		//完成clnt_socks数组的更新，打开互斥锁

		while(clnt_cnt>=MAX_CONNECT);//如果达到最大连接数，忙闲等待，不接受新的连接
	}

	printf("Server offline\n");

	pthread_mutex_destroy(&mutx);
}

void *connect_handle(void *arg)
{
	int sock = *((int *)arg);
	pthread_mutex_unlock(&mutx_clnt_sock);
	//完成clnt_sock的复制，解除互斥锁
	char recv_buf[BUF_SIZE];
	memset(recv_buf, 0, BUF_SIZE);

	while (1)
	{
		int readLen = read(sock, recv_buf, BUF_SIZE);

		if (readLen <= 0)//异常或断开连接
		{
				break;
		}
		printf("read bytes:%d\n",readLen);
		printf("read buf:'%s'\n",recv_buf);
		broadcast_msg(recv_buf, readLen);
	}

	printf("recv error or socket disconnected!\n");

	//清理clnt_socks中对应的socket
	pthread_mutex_lock(&mutx);

	int i;
	for (i = 0; i < clnt_cnt; i++)
	{
		if (clnt_socks[i] == sock)
		{
			printf("delete socket : %d\n", sock);
			break;
		}
	}
	clnt_cnt--;
	for (; i < clnt_cnt; i++)
	{
		clnt_socks[i] = clnt_socks[i + 1];
	}
	printf("sockets reset finished!\n");
	pthread_mutex_unlock(&mutx);
}

void broadcast_msg(const char msg[], int msgLen)
{
	pthread_mutex_lock(&mutx);
	for (int i = 0; i < clnt_cnt; i++)
	{
		if (write(clnt_socks[i], msg, msgLen) < 0)
		{
			printf("error!message boardcast failed!\n");
			return;
		}
	}
	printf("message broadcast finished\n");
	pthread_mutex_unlock(&mutx);
}

void error_handling(const char *msg)
{
	printf("error:%s!\n", msg);
	exit(1);
}
