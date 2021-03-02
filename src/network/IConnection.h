/*
 * Created by chrji on 2021/2/8.
 */

#ifndef TINYGAMESERVER_ICONNECTION_H
#define TINYGAMESERVER_ICONNECTION_H

#include "../include/Base.h"
#include "inner.h"

#include <memory>
#include <atomic>
#include <utility>


class Buffer;

class IConnection
{
public:

    explicit IConnection(LoopSPtr loop)
            : loop_(loop), status_(Connected), id_(-1)
    {
    }

    virtual ~IConnection() noexcept = default;

    ConnStatus GetStatus() const
    {
        return status_;
    }

    std::string GetAddr() const
    {
        return addr_;
    }

    int64_t GetId() const
    {
        return id_;
    }

    std::shared_ptr<Buffer> GetBuffer() const
    {
        return recv_buffer_;
    };

    void SetNewConnectionCallBack(const std::function<int(ConnSPtr)>& cb)
    {
        conn_cb_ = cb;
    }

    void SetReadCallBack(const std::function<int(ConnSPtr)>& cb)
    {
        read_cb_ = cb;
    }

    void SetCloseCallBack(const std::function<int(ConnSPtr)>& cb)
    {
        close_cb_ = cb;
    }

    virtual void Start() = 0;

    // only can be called by client
    virtual void Close() = 0;

    virtual void Send(std::shared_ptr<Buffer>) = 0;

protected:
    LoopSPtr loop_;
    std::atomic<ConnStatus> status_;

    std::string addr_;
    int64_t id_;

    std::shared_ptr<Buffer> recv_buffer_;
    std::shared_ptr<Buffer> send_buffer_;

    std::function<int(ConnSPtr)> conn_cb_;
    std::function<int(ConnSPtr)> read_cb_;
    std::function<int(ConnSPtr)> close_cb_;

private:

};


#endif //TINYGAMESERVER_ICONNECTION_H
