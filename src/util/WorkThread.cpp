/*
 *  Created by chrji on 2021/2/4.
 */

#include <unistd.h>
#include "WorkThread.h"
#include "../include/EventLoop.h"

WorkThread::WorkThread(const std::function<int()>& functor)
        : status_(Initial)
{
    loop_ = std::make_shared<EventLoop>();
    loop_->Init();
    thread_ = std::make_shared<std::thread>([this, functor]()
                                            {
                                                Run(functor);
                                            });

    tid_ = thread_->get_id();

    thread_->detach();
}

WorkThread::~WorkThread()
{
    Stop();
}

void WorkThread::Start()
{
    while (status_ == Initial)
    {
        usleep(1);
    }
}

void WorkThread::Stop()
{
    loop_->Stop();
}

void WorkThread::Run(std::function<int()> functor)
{
    status_ = Running;

    loop_->Run(false);

    status_ = Stopped;
}
