/*
 *  Created by chrji on 2021/1/31.
 */

#ifndef TINYGAMESERVER_TCPCONNECTION_H
#define TINYGAMESERVER_TCPCONNECTION_H

#include "IConnection.h"

#include <memory>
#include <functional>

class FdWatcher;

class Buffer;

class TCPConnection : public std::enable_shared_from_this<TCPConnection>, public IConnection
{
public:

    TCPConnection(LoopSPtr loop,
                  evutil_socket_t fd,
                  const std::string& addr,
                  int64_t id);

    ~TCPConnection() noexcept override;

    void Start() override;

    void Close() override;

    void Send(std::shared_ptr<Buffer> buf) override;


protected:

private:

    int HandleRead();

    int HandleWrite();

    int HandleClose();

//    int SendInLoop();

    int SendInLoop(std::shared_ptr<Buffer> buf);


    evutil_socket_t fd_;

    std::shared_ptr<FdWatcher> fd_watcher_;

};


#endif //TINYGAMESERVER_TCPCONNECTION_H
