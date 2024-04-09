#include <string>
#include <cstring>
#include <iostream>
#include "json.hpp"

//网络相关
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
using namespace std;

const int MAX_CONNECT = 300;
int clnt_cnt;
int clnt_socks[MAX_CONNECT];

void error_handling(const char* msg);

int main()
{
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
	serv_adr.sin_addr.s_addr = inet_addr("0.0.0.0");	
	serv_adr.sin_port = htons(8888);

	int val = 1;
	setsockopt(serv_sock,SOL_SOCKET,SO_REUSEADDR,(void*)&val,sizeof(serv_sock));

	if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr)) == -1)
		error_handling("bind error");
	printf("bind success!\n");
	

	if(listen(serv_sock,5)==-1)
		error_handling("listen error");
	printf("start listening!\n");
	
	while(1)
	{	
		clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_adr,&clnt_adr_size);
		if(clnt_sock == -1)
			error_handling("accept error!\n");
		printf("accept clnt:%s\n",inet_ntoa(clnt_adr.sin_addr));
	}	
	
}


void error_handling(const char* msg)
{
	printf("error:%s!\n",msg);
	exit(1);
}
