/*
 *  Created by chrji on 2021/1/31.
 */

#ifndef TINYGAMESERVER_TCPSERVER_H
#define TINYGAMESERVER_TCPSERVER_H

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <functional>

#include <event2/util.h>

#include "IConnection.h"

class IAcceptor;

class ThreadManager;


class TCPServer
{
public:

    TCPServer(LoopSPtr loop, const std::string& addr, unsigned int thread_num = 8);

    ~TCPServer();

    void Start();


    ConnSPtr GetConnection(uint id)
    {
        return connections_[id];
    }

    void SetConnectionCallBack(const std::function<int(ConnSPtr)>& cb)
    {
        connection_cb_ = cb;
    }

    void SetMessageCallBack(const std::function<int(ConnSPtr)>& cb)
    {
        message_cb_ = cb;
    }

    void SetCloseCallBack(const std::function<int(ConnSPtr)>& cb)
    {
        close_cb_ = cb;
    }

protected:

private:
    void HandleNewConn(evutil_socket_t fd, const std::string& addr);


    LoopSPtr loop_;
    std::string addr_;


    unsigned int thread_num_;
    std::shared_ptr<ThreadManager> threads_;

    std::shared_ptr<IAcceptor> listener_;
    std::unordered_map<int64_t , ConnSPtr> connections_;

    std::function<int(ConnSPtr)> connection_cb_;
    std::function<int(ConnSPtr)> message_cb_;
    std::function<int(ConnSPtr)> close_cb_;
};


#endif //TINYGAMESERVER_TCPSERVER_H
