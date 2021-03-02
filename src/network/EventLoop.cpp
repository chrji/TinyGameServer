/*
 *  Created by chrji on 2021/2/1.
 */

#include "../include/EventLoop.h"
#include "../util/EventChannel.h"

EventLoop::EventLoop()
        : evbase_(event_base_new()),
          tid_(std::this_thread::get_id()),
          status_(Initial)
{
}

EventLoop::~EventLoop()
{
    Stop();

    event_base_free(evbase_);
    evbase_ = nullptr;
}

void EventLoop::Init()
{
    channel_ = std::make_shared<EventChannel>(shared_from_this());
    channel_->SetAsyncTask([this]() -> int
                           { return ProcessPendingFunctors(); });
}

void EventLoop::Stop()
{
    if (status_ == Running)
    {
        status_ = Stopped;

        while (!pending_functors_.empty())
        {
            ProcessPendingFunctors();
        }

        event_base_loopexit(evbase_, nullptr);

        while (!pending_functors_.empty())
        {
            ProcessPendingFunctors();
        }
    }
}

void EventLoop::Run(bool exit_on_empty)
{
    status_ = Running;
    tid_ = std::this_thread::get_id();

    int flag = exit_on_empty ? 0 : EVLOOP_NO_EXIT_ON_EMPTY;
    event_base_loop(evbase_, flag);

//    event_base_dispatch(evbase_);

    status_ = Stopped;
}

void EventLoop::AsyncCall(const std::function<int()>& functor)
{
    {
        std::lock_guard<std::mutex> lock(mutex_functors_);
        pending_functors_.emplace_back(functor);
    }
    channel_->Notify();
}

void EventLoop::AsyncCall(std::function<int()>&& functor)
{
    {
        std::lock_guard<std::mutex> lock(mutex_functors_);
        pending_functors_.emplace_back(std::move(functor));
    }
    channel_->Notify();
}

int EventLoop::ProcessPendingFunctors()
{
    std::vector<std::function<int()>> functors;
    {
        std::lock_guard<std::mutex> lock(mutex_functors_);
        pending_functors_.swap(functors);
    }

    for (auto& f: functors)
    {
        f();
    }

    return 0;
}

