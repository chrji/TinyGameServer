/*
 *  Created by chrji on 2021/2/1.
 */

#ifndef TINYGAMESERVER_EVENTLOOP_H
#define TINYGAMESERVER_EVENTLOOP_H

#include <thread>
#include <functional>

#include <event2/event.h>

#include "Base.h"

class EventChannel;

class EventLoop : public std::enable_shared_from_this<EventLoop>
{
public:

    EventLoop();

    ~EventLoop();

    void Init();

    void Stop();

    void Run(bool exit_on_empty = false);

    void AsyncCall(const std::function<int()>& functor);

    void AsyncCall(std::function<int()>&& functor);

    event_base* GetBase()
    { return evbase_; }

    std::thread::id GetTid() const
    { return tid_; }

    LoopStatus GetStatus() const
    { return status_; }


protected:

private:



    int ProcessPendingFunctors();


    event_base* evbase_;
    std::thread::id tid_;
    std::atomic<LoopStatus> status_;

    std::shared_ptr<EventChannel> channel_;

    std::mutex mutex_functors_;
    std::vector<std::function<int()>> pending_functors_;


};


#endif //TINYGAMESERVER_EVENTLOOP_H
