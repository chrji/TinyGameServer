/*
 *  Created by chrji on 2021/1/31.
 */

#include "TCPConnection.h"

#include "util/Buffer.h"
#include "include/EventLoop.h"
#include "util/FdWatcher.h"

#include <stdio.h>
#include <unistd.h>

#include <utility>

TCPConnection::TCPConnection(LoopSPtr loop, evutil_socket_t fd, const std::string& addr, int64_t id)
        : IConnection(loop),
          fd_(fd)
{
    status_ = Connected;
    serial_id_ = id;
    addr_ = addr;

    recv_buffer_ = std::make_shared<Buffer>();
    send_buffer_ = std::make_shared<Buffer>();

    fd_watcher_ = std::make_shared<FdWatcher>(loop_, fd_, 0);

    fd_watcher_->SetReadCallBack([this]() -> int
                                 {
                                     return HandleRead();
                                 });

    fd_watcher_->SetWriteCallBack([this]() -> int
                                  {
                                      return HandleWrite();
                                  });
}

TCPConnection::~TCPConnection() noexcept
{
    status_ = Disconnected;
    evutil_closesocket(fd_);
}

void TCPConnection::Start()
{
    fd_watcher_->EnableReadEvent();

    if (conn_cb_)
    {
        conn_cb_(shared_from_this());
    }
}

void TCPConnection::Close()
{
    auto conn = shared_from_this();
    loop_->AsyncCall([conn]() -> int
                     {
                         return conn->HandleClose();
                     });
}

void TCPConnection::Send(std::shared_ptr<Buffer> buf)
{
    if (status_ != Connected)
    {
        return;
    }

    loop_->AsyncCall([this, buf]() -> int
                     {
                         return SendInLoop(buf);
                     });
}

int TCPConnection::HandleRead()
{
    int len = recv_buffer_->ReadFd(fd_);
    if (len > 0)
    {
        read_cb_(shared_from_this());
    }
    else if (len == 0)
    {
        HandleClose();
    }
    else
    {
        // TODO handle error
    }

    return 0;
}

int TCPConnection::HandleWrite()
{
    auto len = write(fd_, send_buffer_->GetData(), send_buffer_->GetLength());
    if (len > 0)
    {
        send_buffer_->Forward(len);
        if (send_buffer_->GetLength() == 0)
        {
            fd_watcher_->DisableWriteEvent();
        }
    }
    else
    {
        // TODO handle error
    }

    return 0;
}

int TCPConnection::HandleClose()
{
    fd_watcher_->DisableAllEvent();
    close_cb_(shared_from_this());

    status_ = Disconnected;

    return 0;
}


int TCPConnection::SendInLoop(std::shared_ptr<Buffer> buf)
{
    int remaining = buf->GetLength();

    if (!fd_watcher_->IsWriting() && send_buffer_->GetLength() == 0)
    {
        int len = write(fd_, buf->GetData(), buf->GetLength());
        if (len >= 0)
        {
            remaining = remaining - len;
            buf->Forward(len);
        }
        else // len < 0
        {
            // TODO handle error
        }
    }

    if (remaining > 0)
    {
        send_buffer_->AppendBuffer(buf);
    }

    if (!fd_watcher_->IsWriting())
    {
        fd_watcher_->EnableWriteEvent();
    }

    return 0;

}