/*
 *  Created by chrji on 2021/2/4.
 */

#include "ThreadManager.h"
#include "../include/EventLoop.h"
#include "../util/WorkThread.h"


ThreadManager::ThreadManager(LoopSPtr loop)
        : loop_(loop), status_(Initial)
{
}


ThreadManager::~ThreadManager()
{
}

void ThreadManager::Start(unsigned int thread_num)
{
    if (thread_num == 0)
    {
        status_ = Running;
        return;
    }

    for (int i = 0; i < thread_num; i++)
    {
        auto thread = std::make_shared<WorkThread>([]() -> int
                                                   {
                                                       return 0;
                                                   });
        threads_.push_back(std::move(thread));
    }
    status_ = Running;
}


void ThreadManager::Stop()
{
    for (auto& t: threads_)
    {
        t->Stop();
    }
    status_ = Stopped;
}

LoopSPtr ThreadManager::PickEventLoop()
{
    static int i = 0;
    LoopSPtr l = loop_;
    if (!threads_.empty())
    {
        l = threads_[i++ % threads_.size()]->GetEventLoop();
    }
    return l;
}