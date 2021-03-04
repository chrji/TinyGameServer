/*
 *  Created by chrji on 2021/2/2.
 */

#ifndef TINYGAMESERVER_IACCEPTOR_H
#define TINYGAMESERVER_IACCEPTOR_H


#include "include/Base.h"
#include "inner.h"

#include <memory>

#include <event2/util.h>

class EventLoop;

class IAcceptor
{
public:

    explicit IAcceptor(LoopSPtr loop)
            : loop_(loop), fd_(-1)
    {};

    virtual ~IAcceptor() = default;

    virtual void Listen() = 0;

    virtual void Accept() = 0;

    virtual void Stop() = 0;

    void SetNewConnectionCallBack(const std::function<int(evutil_socket_t, const std::string&)>& cb)
    {
        new_connection_cb_ = cb;
    }

protected:
    LoopSPtr loop_;
    evutil_socket_t fd_;
    std::string addr_;

    std::function<int(evutil_socket_t, const std::string&)> new_connection_cb_;

private:

};


#endif //TINYGAMESERVER_IACCEPTOR_H
