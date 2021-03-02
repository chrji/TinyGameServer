/*
 * Created by chrji on 2021/2/27.
 */

#ifndef TINYGAMESERVER_TCPCLIENT_H
#define TINYGAMESERVER_TCPCLIENT_H

#include <string>
#include <memory>
#include <functional>

#include "IConnection.h"

#include "inner.h"

class EventLoop;

class IConnector;

class IConnection;


class TCPClient
{
public:

    TCPClient(LoopSPtr loop, const std::string& addr);

    ~TCPClient();

    void Connect();

    void Disconnect();


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

    std::shared_ptr<IConnector> connector_;
    std::shared_ptr<IConnection> connection_;

    std::function<int(ConnSPtr)> connection_cb_;
    std::function<int(ConnSPtr)> message_cb_;
    std::function<int(ConnSPtr)> close_cb_;
};


#endif //TINYGAMESERVER_TCPCLIENT_H
