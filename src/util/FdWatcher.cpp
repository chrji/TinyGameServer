/*
 *  Created by chrji on 2021/2/5.
 */

#include "FdWatcher.h"
#include "../include/EventLoop.h"

#include <event2/event_compat.h>

FdWatcher::FdWatcher(LoopSPtr loop, int fd, int ev_type)
        : loop_(loop), fd_(fd), ev_type_(0)
{
    ev_type_ = (ev_type & EV_READ) | (ev_type & EV_WRITE);
    event_ = new event;
    added_ = false;
}

FdWatcher::FdWatcher(const FdWatcher& f)
        : loop_(f.loop_), fd_(f.fd_), ev_type_(f.ev_type_)
{
    event_ = new event;
    added_ = false;

    if (f.read_cb_)
    {
        read_cb_ = f.read_cb_;
    }

    if (f.write_cb_)
    {
        write_cb_ = f.write_cb_;
    }
}

FdWatcher::FdWatcher(FdWatcher&& f) noexcept
        : loop_(std::move(f.loop_)), fd_(f.fd_), ev_type_(f.ev_type_)
{
    event_ = f.event_;
    f.event_ = nullptr;
    added_.store(f.added_);

    if (f.read_cb_)
    {
        read_cb_ = f.read_cb_;
    }

    if (f.write_cb_)
    {
        write_cb_ = f.write_cb_;
    }
}

FdWatcher& FdWatcher::operator=(const FdWatcher& f)
{
    loop_ = f.loop_;
    fd_ = f.fd_;
    ev_type_ = f.ev_type_;

    event_ = new event;
    added_ = false;

    if (f.read_cb_)
    {
        read_cb_ = f.read_cb_;
    }

    if (f.write_cb_)
    {
        write_cb_ = f.write_cb_;
    }
    return *this;
}

FdWatcher& FdWatcher::operator=(FdWatcher&& f) noexcept
{
    loop_ = std::move(f.loop_);
    fd_ = f.fd_;
    ev_type_ = f.ev_type_;

    event_ = f.event_;
    f.event_ = nullptr;
    added_.store(f.added_);

    if (f.read_cb_)
    {
        read_cb_ = f.read_cb_;
    }

    if (f.write_cb_)
    {
        write_cb_ = f.write_cb_;
    }
    return *this;
}

FdWatcher::~FdWatcher()
{
    if (added_)
    {
        event_del(event_);
    }
    delete event_;
}

void FdWatcher::Start()
{
    if (added_)
    {
        event_del(event_);
    }

    event_set(event_, fd_, ev_type_ | EV_PERSIST, &FdWatcher::Handle, this);
    event_base_set(loop_->GetBase(), event_);

    event_add(event_, nullptr);
    added_ = true;
}

void FdWatcher::Handle(evutil_socket_t fd, short which, void* arg)
{
    auto pWatcher = static_cast<FdWatcher*>(arg);

    if ((which & EV_READ) && pWatcher->read_cb_)
    {
        pWatcher->read_cb_();
    }

    if ((which & EV_WRITE) && pWatcher->write_cb_)
    {
        pWatcher->write_cb_();
    }
}
