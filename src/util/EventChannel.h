/*
 * Created by chrji on 2021/2/7.
 */

#ifndef TINYGAMESERVER_EVENTCHANNEL_H
#define TINYGAMESERVER_EVENTCHANNEL_H


#include <string.h>

#include <memory>
#include <functional>
#include <atomic>

#include <event2/event.h>

#include "network/inner.h"


class EventChannel
{
public:
    EventChannel(LoopSPtr loop);

    ~EventChannel();

    void SetAsyncTask(const std::function<int()>& functor);

    void SetAsyncTask(std::function<int()>&& functor);

    void Notify();

protected:

private:
    void InitEvent();

    static void Update(evutil_socket_t fd, short which, void* arg);

    LoopSPtr loop_;
    std::shared_ptr<event> event_;
    std::atomic<bool> added_;

    evutil_socket_t fds_[2]; // simulation of linux pipe

    std::function<int()> functor_;

};


#endif //TINYGAMESERVER_EVENTCHANNEL_H
