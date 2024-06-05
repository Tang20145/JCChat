#include "server.h"

Server::Server()
{

}

Server::~Server()
{

}

//创建监听对象
void Server::listen(const char * ip,int port)
{
    evconnlistener_new_bind();
}