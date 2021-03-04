/*
 * Created by chrji on 2021/2/7.
 */

#include "EventChannel.h"
#include "include/EventLoop.h"

#include <event2/event_compat.h>


EventChannel::EventChannel(LoopSPtr loop)
        : loop_(loop)
{
    event_ = std::make_shared<event>();
    memset(fds_, 0, sizeof(fds_));
    added_ = false;

    evutil_socketpair(AF_UNIX, SOCK_STREAM, 0, fds_);
    evutil_make_socket_nonblocking(fds_[0]);
    evutil_make_socket_nonblocking(fds_[1]);

}

EventChannel::~EventChannel()
{
    if (fds_[0] > 0)
    {
        EVUTIL_CLOSESOCKET(fds_[0]);
        EVUTIL_CLOSESOCKET(fds_[1]);
        memset(fds_, 0, sizeof(fds_));
    }

}

void EventChannel::SetAsyncTask(const std::function<int()>& functor)
{
    functor_ = functor;
    InitEvent();
}

void EventChannel::SetAsyncTask(std::function<int()>&& functor)
{
    functor_ = std::move(functor);
    InitEvent();
}

void EventChannel::Notify()
{
    char buf[] = "";
    send(fds_[1], buf, sizeof(buf), 0);
}

void EventChannel::InitEvent()
{
    if (added_)
    {
        event_del(event_.get());
    }

    event_assign(event_.get(), loop_->GetBase(), fds_[0],
                 EV_READ | EV_PERSIST, &EventChannel::Update, this);

    event_add(event_.get(), nullptr);
    added_ = true;
}


void EventChannel::Update(evutil_socket_t fd, short which, void* arg)
{
    EventChannel* e = static_cast<EventChannel*>(arg);

    char buf[64];
    while (recv(fd, buf, sizeof(buf), 0) > 0);

    e->functor_();
}

