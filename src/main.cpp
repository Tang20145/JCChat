#include <iostream>
#include <string>
#include <cstring>
#include "json.hpp"
#include <unistd.h>

//多线程相关
#include <pthread.h>

//网络相关
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
using namespace std;

const int BUF_SIZE = 1024;
const int MAX_CONNECT = 300;
int clnt_cnt;//当前连接的客户端socket
int clnt_socks[MAX_CONNECT];//数组存储所有已连接的socket

char msg_buf[BUF_SIZE];
char json_buf[BUF_SIZE*2];

void error_handling(const char* msg);

pthread_mutex_t mutx;
void* connect_handle(void* arg);//用于创建处理连接的线程
void broadcast_msg(const char msg[],int msg_len);//广播消息函数


int main()
{
//互斥锁初始化
	pthread_mutex_init(&mutx,NULL);

//tcp基础设置
	int clnt_sock;
	struct sockaddr_in clnt_adr;
	socklen_t clnt_adr_size = sizeof(clnt_adr);	


	int serv_sock = socket(PF_INET,SOCK_STREAM,0);
	if(serv_sock == -1)
		error_handling("socket error");	
	printf("socket success!\n");	
	
	
	struct sockaddr_in serv_adr;
	memset(&serv_adr,0,sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr("10.0.20.9");	
	serv_adr.sin_port = htons(8000);

	int val = 1;
	setsockopt(serv_sock,SOL_SOCKET,SO_REUSEADDR,(void*)&val,sizeof(serv_sock));

	if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr)) == -1)
		error_handling("bind error");
	printf("bind success!\n");
	

	if(listen(serv_sock,5)==-1)
		error_handling("listen error");
	printf("start listening!\n");
//循环接收连接	
	while(1)
	{	
		
		clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_adr,&clnt_adr_size);
		if(clnt_sock == -1)
			error_handling("accept error!\n");
		printf("accept clnt:%s\n",inet_ntoa(clnt_adr.sin_addr));
		
		//互斥锁
		pthread_mutex_lock(&mutx);
			
		pthread_t thread_id;
		if(pthread_create(&thread_id,NULL,connect_handle,(void*)&clnt_sock) == 0)	
			printf("create connect handle thread success! Thread id : %lu\n",thread_id);
		else
			error_handling("create connect handle thread failed");
		clnt_socks[clnt_cnt] = clnt_sock;
		clnt_cnt++;
		pthread_mutex_unlock(&mutx);
	}	
	

	pthread_mutex_destroy(&mutx);	
}

void* connect_handle(void* arg)
{
	int sock = *((int*)arg);
	char recv_buf[BUF_SIZE];	
	memset(recv_buf,0,BUF_SIZE);
	int recv_len;
	while((recv_len = read(sock,recv_buf,BUF_SIZE))>0)
	{
		
		printf("recv %d bytes\n",recv_len);
		printf("recv buf : %s\n",recv_buf);
		printf("start broadcasting......\n");
		broadcast_msg(recv_buf,recv_len);		
		memset(recv_buf,0,recv_len);
	}
	
}

void broadcast_msg(const char msg[],int msg_len)
{
	pthread_mutex_lock(&mutx);
	int msg_front_len = *((int*)&msg[0]);
	printf("broadcast msg first 4 bytes = %d\n",msg_front_len);
	printf("message :%s\n",msg+4);
	for(int i = 0 ; i < clnt_cnt ; i++)
	{
		if(write(clnt_socks[i],msg,msg_len)<0)
		{
			printf("error!message boardcast failed!\n");
			return ;
		}
	}
	printf("message broadcast finished\n");	
	pthread_mutex_unlock(&mutx);		
}

void error_handling(const char* msg)
{
	printf("error:%s!\n",msg);
	exit(1);
}
