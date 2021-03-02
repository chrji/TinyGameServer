/*
 *  Created by chrji on 2021/2/2.
 */

#include "TCPAcceptor.h"
#include "../include/EventLoop.h"
#include "../util/FdWatcher.h"

//#include "../include/common.h"

#include <cstring>

#include <string>


TCPAcceptor::TCPAcceptor(LoopSPtr loop, const std::string& addr)
        : IAcceptor(loop)
{
    addr_ = addr;
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
}

TCPAcceptor::~TCPAcceptor() noexcept
{
    evutil_closesocket(fd_);
}

void TCPAcceptor::Listen()
{
    evutil_make_socket_nonblocking(fd_);

    // set keepalive
    const int on = 1;
    setsockopt(fd_,
               SOL_SOCKET,
               SO_KEEPALIVE,
               reinterpret_cast<const void*>(&on),
               static_cast<socklen_t>(sizeof(on)));

    // set reuse addr and addr
    evutil_make_listen_socket_reuseable(fd_);
    evutil_make_listen_socket_reuseable_port(fd_);


    auto host_port = SplitHostPort(addr_);

    sockaddr_in server_addr{};
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(host_port.second);

    bind(fd_, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));

    listen(fd_, SOMAXCONN);
}


void TCPAcceptor::Accept()
{
    fd_watcher_ = std::make_shared<FdWatcher>(loop_, fd_, EV_READ);
    fd_watcher_->SetReadCallBack([this]() -> int
                                 {
                                     HandleAccept();
                                     return 0;
                                 });

    loop_->AsyncCall([this]() -> int
                     {
                         fd_watcher_->Start();
                         return 0;
                     });
}

void TCPAcceptor::Stop()
{
    fd_watcher_->DisableAllEvent();
}

void TCPAcceptor::HandleAccept()
{
    sockaddr_in addr{};
    unsigned int len = sizeof(addr);

    int nfd = -1;
    nfd = accept(fd_, reinterpret_cast<sockaddr*>(&addr), &len);
    if (nfd == -1)
    {
        return;
    }

    // set  nonblocking
    evutil_make_socket_nonblocking(nfd);

    // set keepalive
    const int on = 1;
    setsockopt(nfd,
               SOL_SOCKET,
               SO_KEEPALIVE,
               reinterpret_cast<const void*>(&on),
               static_cast<socklen_t>(sizeof(on)));


    if (new_connection_cb_)
    {
        char buf[16] = {};
        evutil_inet_ntop(addr.sin_family, &addr.sin_addr, buf, 16);
        new_connection_cb_(nfd, std::string(buf));
    }
}
