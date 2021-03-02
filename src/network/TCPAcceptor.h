/*
 *  Created by chrji on 2021/2/2.
 */

#ifndef TINYGAMESERVER_TCPACCEPTOR_H
#define TINYGAMESERVER_TCPACCEPTOR_H

#include "IAcceptor.h"

#include <memory>

#include <event2/event.h>

class EventLoop;

class FdWatcher;

class TCPAcceptor : public IAcceptor
{
public:
    TCPAcceptor(LoopSPtr loop, const std::string& addr);

    virtual ~TCPAcceptor() noexcept;

    virtual void Listen() override;

    virtual void Accept() override;

    virtual void Stop() override;


protected:

private:
    void HandleAccept();


    std::shared_ptr<FdWatcher> fd_watcher_;


};


#endif //TINYGAMESERVER_TCPACCEPTOR_H
