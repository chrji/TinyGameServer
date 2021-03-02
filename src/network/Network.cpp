/*
 *  Created by chrji on 2021/1/22.
 */

#include "../include/Network.h"

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <cstdlib>
#include <cstring>
#include <cassert>

#include <event2/listener.h>
#include <event2/bufferevent.h>

class NetworkPrivate
{
public:
    struct CallBackArg
    {
        event_base* base;
        void* arg;
    };

    sockaddr_in sin{};

    event_base* base{};
    evconnlistener* listener{};

    CallBackArg* acceptObj{};
    CallBackArg* revcObj{};
    CallBackArg* closeObj{};
    std::function<void(unsigned char, int, void*)> accept;
    std::function<void(unsigned char, int, void*)> recv;
    std::function<void(unsigned char, int, void*)> close;


};

void socket_read_cb(bufferevent *bev, void *arg);
void socket_event_cb(bufferevent *bev, short events, void *arg);

void
listener_cb(struct evconnlistener* listener, int fd, struct sockaddr* sock, int socklen, void* arg)
{

    printf("accept a client %d\n", fd);

    event_base* base = (event_base*) arg;

    //下面代码是为这个fd创建一个bufferevent
    bufferevent* bev = bufferevent_socket_new(base, fd,
                                              BEV_OPT_CLOSE_ON_FREE);

    //bufferevent_setcb(bev, socket_read_cb, NULL, socket_event_cb, NULL);
    bufferevent_enable(bev, EV_READ | EV_PERSIST);

}


Network::Network()
{
    this->p = new NetworkPrivate;
}

Network::~Network()
{
    delete p;
}


void Network::Init()
{

}

void Network::Listen(int port)
{
    assert(port > 0 && port <= 65535);

    sockaddr_in sin = p->sin;
    memset(&sin, 0, sizeof(struct sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);

    p->base = event_base_new();

    p->acceptObj->base = p->base;
    p->revcObj->base = p->base;
    p->closeObj->base = p->base;

    p->listener = evconnlistener_new_bind(p->base, listener_cb, p->acceptObj,
                                          LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
                                          10, (struct sockaddr*) &sin,
                                          sizeof(struct sockaddr_in));
//    event_base_loop();

}

void Network::OnAccept(std::function<void(unsigned char, int, void*)> func, void* arg)
{
    p->accept = func;
    p->acceptObj->arg = arg;
}

void Network::OnRecv(std::function<void(unsigned char, int, void*)> func, void* arg)
{
    p->recv = func;
    p->revcObj->arg = arg;
}

void Network::OnClose(std::function<void(unsigned char, int, void*)> func, void* arg)
{
    p->close = func;
    p->closeObj->arg = arg;
}

