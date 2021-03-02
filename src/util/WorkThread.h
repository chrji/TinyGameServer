/*
 *  Created by chrji on 2021/2/4.
 *
 */

#ifndef TINYGAMESERVER_WORKTHREAD_H
#define TINYGAMESERVER_WORKTHREAD_H

#include "../include/Base.h"
#include "../network/inner.h"

#include <memory>
#include <mutex>
#include <thread>
#include <functional>



class EventLoop;

class WorkThread
{
public:

    explicit WorkThread(const std::function<int(void)>& functor);

    ~WorkThread();

    void Start();

    void Stop();


    LoopSPtr GetEventLoop()
    { return loop_; }

    std::thread::id GetTid() const
    { return tid_; };

    LoopStatus GetStatus() const
    {
        return status_;
    }

protected:

private:
    void Run(std::function<int(void)> functor);


    LoopSPtr loop_;
//    std::function<int(void)> functor_;

    std::mutex mutex_;
    std::shared_ptr<std::thread> thread_;
    std::thread::id tid_;

    std::atomic<LoopStatus> status_;


};


#endif //TINYGAMESERVER_WORKTHREAD_H
