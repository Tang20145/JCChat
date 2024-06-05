#ifndef SERVER_H_
#define SERVER_H_

//libevent¿â
#include <event.h>

#define IP "10.0.20.9"
#define PORT 8000

class Server
{
    private:
    struct event_base * base;

    public:
    Server();
    ~Server();

    void listen(const char *,int);

};




#endif