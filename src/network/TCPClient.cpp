/*
 * Created by chrji on 2021/2/27.
 */

#include "TCPClient.h"
#include "TCPConnector.h"
#include "TCPConnection.h"
#include "include/EventLoop.h"

TCPClient::TCPClient(LoopSPtr loop, const std::string& addr)
        : loop_(loop),
          addr_(addr)
{
    connector_ = std::make_shared<TCPConnector>(loop_, addr);
}

TCPClient::~TCPClient()
{

}

void TCPClient::Connect()
{
    connector_->SetNewConnectionCallBack([this](evutil_socket_t fd, const std::string& addr) -> int
                                         {
                                             HandleNewConn(fd, addr);
                                             return 0;
                                         });
    connector_->Connect();
}

void TCPClient::Disconnect()
{
    connection_->Close();
}

void TCPClient::HandleNewConn(int fd, const std::string& addr)
{
    auto conn = std::make_shared<TCPConnection>(loop_, fd, addr, -1);
    conn->SetNewConnectionCallBack(connection_cb_);
    conn->SetReadCallBack(message_cb_);
    conn->SetCloseCallBack([this](const ConnSPtr& c) -> int
                           {
                               connection_.reset();
                               return 0;
                           });

    loop_->AsyncCall([this]() -> int
                     {
                         connection_->Start();
                         return 0;
                     });

    connection_ = conn;

}
