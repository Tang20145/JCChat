#include "server.h"

Server::Server()
{

}

Server::~Server()
{

}

//������������
void Server::listen(const char * ip,int port)
{
    evconnlistener_new_bind();
}