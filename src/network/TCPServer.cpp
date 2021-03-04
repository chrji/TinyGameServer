/*
 *  Created by chrji on 2021/1/31.
 */

#include "TCPServer.h"

#include "include/EventLoop.h"
#include "util/ThreadManager.h"
#include "network/TCPAcceptor.h"
#include "network/TCPConnection.h"
#include "util/UID.h"


TCPServer::TCPServer(LoopSPtr loop, const std::string& addr, unsigned int thread_num)
        : loop_(loop),
          addr_(addr),
          thread_num_(thread_num)
{
    threads_ = std::make_shared<ThreadManager>(loop);
    listener_ = std::make_shared<TCPAcceptor>(loop, addr);
}

TCPServer::~TCPServer()
{
    threads_->Stop();
}

void TCPServer::Start()
{
    threads_->Start(thread_num_);
    listener_->Listen();

    listener_->SetNewConnectionCallBack([this](evutil_socket_t fd, const std::string& addr) -> int
                                        {
                                            HandleNewConn(fd, addr);
                                            return 0;
                                        });
    listener_->Accept();

}

void TCPServer::HandleNewConn(evutil_socket_t fd, const std::string& addr)
{
    auto loop = threads_->PickEventLoop();
    auto serial_id = UID::GenerateSerialID();

    ConnSPtr conn = std::make_shared<TCPConnection>(loop, fd, addr, serial_id);

    conn->SetNewConnectionCallBack(connection_cb_);
    conn->SetReadCallBack(message_cb_);
    conn->SetCloseCallBack([this](const ConnSPtr& c) -> int
                           {
                               connections_.erase(c->GetSerialId());
                               if (close_cb_)
                               {
                                   close_cb_(c);
                               }
                               return 0;
                           });

    connections_[serial_id] = conn;

    loop->AsyncCall([this,serial_id]() -> int
                    {
                        connections_[serial_id]->Start();
                        return 0;
                    });


}

