/*
 *  Created by chrji on 2021/2/5.
 */

#ifndef TINYGAMESERVER_FDWATCHER_H
#define TINYGAMESERVER_FDWATCHER_H

#include <functional>
#include <memory>
#include <atomic>

#include <event2/event.h>
#include "../network/inner.h"

class EventLoop;

class FdWatcher
{
public:
    FdWatcher(LoopSPtr loop, evutil_socket_t fd, int ev_type);

    FdWatcher(const FdWatcher& f);

    FdWatcher(FdWatcher&& f) noexcept ;

    FdWatcher& operator=(const FdWatcher& f);

    FdWatcher& operator=(FdWatcher&& f) noexcept ;

    ~FdWatcher();

    void Start();

    void EnableReadEvent()
    {
        if (!IsReading()) {
            ev_type_ |= EV_READ;
            Start();
        }
    }

    void EnableWriteEvent()
    {
        if (!IsWriting()) {
            ev_type_ |= EV_WRITE;
            Start();
        }
    }

    void DisableAllEvent()
    {
        ev_type_ = 0;
        event_del(event_);
    }

    void DisableReadEvent()
    {
        if (IsReading()) {
            ev_type_ &= ~EV_READ;
            Start();
        }
    }

    void DisableWriteEvent()
    {
        if (IsWriting()) {
            ev_type_ &= ~EV_WRITE;
            Start();
        }
    }

    bool IsWriting() const
    { return ev_type_ & EV_WRITE; }

    bool IsReading() const
    { return ev_type_ & EV_READ; }

    void SetReadCallBack(const std::function<int()>& cb)
    { read_cb_ = cb; }

    void SetWriteCallBack(const std::function<int()>& cb)
    { write_cb_ = cb; }

protected:

private:

    static void Handle(evutil_socket_t fd, short which, void* arg);

    LoopSPtr loop_;
    evutil_socket_t fd_;

    int ev_type_;
    event* event_;
    std::atomic<bool> added_;

    std::function<int()> read_cb_;
    std::function<int()> write_cb_;
};


#endif //TINYGAMESERVER_FDWATCHER_H
