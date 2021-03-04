/*
 * Created by chrji on 2021/2/8.
 */

#ifndef TINYGAMESERVER_ICONNECTION_H
#define TINYGAMESERVER_ICONNECTION_H

#include "include/Base.h"
#include "inner.h"

#include <memory>
#include <atomic>
#include <utility>


class Buffer;

class IConnection
{
public:

    explicit IConnection(LoopSPtr loop)
            : loop_(loop),
              status_(Connected),
              serial_id_(-1),
              user_id_(-1),
              authorized_(false)
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

    int64_t GetSerialId() const
    {
        return serial_id_;
    }

    int64_t getUserId() const
    {
        return user_id_;
    }

    void setUserId(int64_t userId)
    {
        user_id_ = userId;
    }

    bool IsAuthorized() const
    {
        return authorized_;
    }

    void SetAuthorized(bool authorized)
    {
        authorized_ = authorized;
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

    virtual void Close() = 0;

    virtual void Send(std::shared_ptr<Buffer>) = 0;

protected:
    LoopSPtr loop_;
    std::atomic<ConnStatus> status_;

    std::string addr_;
    int64_t serial_id_;
    int64_t user_id_;

    bool authorized_;

    std::shared_ptr<Buffer> recv_buffer_;
    std::shared_ptr<Buffer> send_buffer_;

    std::function<int(ConnSPtr)> conn_cb_;
    std::function<int(ConnSPtr)> read_cb_;
    std::function<int(ConnSPtr)> close_cb_;

private:

};


#endif //TINYGAMESERVER_ICONNECTION_H
