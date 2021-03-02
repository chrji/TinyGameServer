/*
 * Created by chrji on 2021/2/17.
 */

#include "TCPConnector.h"
#include "../include/EventLoop.h"


#include <string.h>

#include <string>

TCPConnector::TCPConnector(LoopSPtr loop, const std::string& addr)
        : IConnector(loop)
{
    addr_ = addr;
}

TCPConnector::~TCPConnector()
{
    if (fd_ != -1)
    {
        evutil_closesocket(fd_);
    }
}


void TCPConnector::Connect()
{
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
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
    server_addr.sin_port = htons(host_port.second);

    evutil_inet_pton(AF_INET, host_port.first.c_str(), &server_addr.sin_addr);

    connect(fd_, reinterpret_cast<const sockaddr*>(&server_addr), sizeof(server_addr));


    loop_->AsyncCall([this]() -> int
                     {
                         new_connection_cb_(fd_, addr_);
                         fd_ = -1;
                         return 0;
                     });

}

void TCPConnector::HandleNewConnection()
{

}

