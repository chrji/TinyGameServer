/*
 *  Created by chrji on 2021/2/4.
 */

#ifndef TINYGAMESERVER_THREADMANAGER_H
#define TINYGAMESERVER_THREADMANAGER_H


#include <vector>
#include <memory>
#include <thread>
#include <functional>

#include "include/Base.h"

#include "network/inner.h"

class EventLoop;

class WorkThread;

class ThreadManager
{
public:
    explicit ThreadManager(LoopSPtr loop);

    ~ThreadManager();

    void Start(unsigned int thread_num = 8);

    void Stop();

    LoopSPtr PickEventLoop();


    LoopSPtr GetEventLoop()
    { return loop_; }

    std::thread::id GetTid() const
    { return tid_; };

    LoopStatus GetStatus() const
    { return status_; }

protected:

private:
    void Run(std::function<int(void)> functor);

    LoopSPtr loop_;
    std::vector<std::shared_ptr<WorkThread>> threads_;
    std::thread::id tid_;

    std::atomic<LoopStatus> status_;
};


#endif //TINYGAMESERVER_THREADMANAGER_H
