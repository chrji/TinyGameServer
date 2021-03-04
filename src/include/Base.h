/*
 *  Created by chrji on 2021/2/5.
 */

#ifndef TINYGAMESERVER_BASE_H
#define TINYGAMESERVER_BASE_H

#include <map>
#include <mutex>
#include <vector>
#include <thread>
#include <memory>
#include <atomic>
#include <functional>

#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/event_compat.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>


enum LoopStatus
{
    Initial = 0,
    Running = 1,
    Stopped = 2
};

enum ConnStatus
{
    Disconnected = 0,
    Connecting = 1,
    Connected = 2
};

enum BufferStatus
{
    READ_SUCCESS = 0,
    READ_INCOMPLETE = 1,
    READ_TYPE_ERROR = 2,
    READ_INNER_ERROR = 3
};


// keyword defer : language feature from go

#define CONTACT(a, b) a##b

#define Defer_(line, code) \
class CONTACT(Defer_,line) \
{ public: \
    std::function<void()> f_; \
    CONTACT(Defer_,line)(std::function<void()>&& f):f_(std::move(f)){} \
    ~CONTACT(Defer_,line)(){f_();} \
} CONTACT(Defer__instance_,line)([&](){code})

#define defer(code)   Defer_(__LINE__,code)


static std::pair<std::string, int> SplitHostPort(const std::string& addr)
{
    std::pair<std::string, int> ret = {"", -1};
    int index = addr.rfind(':');
    if (index == std::string::npos)
    {
        return ret;
    }

    ret.first = std::string(addr, 0, index);
    ret.second = atoi(&addr[index + 1]);

    return ret;
}

#endif //TINYGAMESERVER_BASE_H
