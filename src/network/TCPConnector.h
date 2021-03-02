/*
 * Created by chrji on 2021/2/17.
 */

#ifndef TINYGAMESERVER_TCPCONNECTOR_H
#define TINYGAMESERVER_TCPCONNECTOR_H

#include "IConnector.h"

#include <memory>

#include <event2/event.h>

class EventLoop;

class FdWatcher;

class TCPConnector : public IConnector
{
public:
    TCPConnector(LoopSPtr loop, const std::string& addr);

    virtual ~TCPConnector();

    virtual void Connect() override;


protected:

private:
    void HandleNewConnection();

    std::shared_ptr<FdWatcher> fd_watcher_;


};


#endif //TINYGAMESERVER_TCPCONNECTOR_H
