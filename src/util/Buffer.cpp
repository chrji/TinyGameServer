/*
 *  Created by chrji on 2021/1/31.
 */

#include "Buffer.h"

#include <string.h>
#include <sys/uio.h>


Buffer::Buffer(unsigned int capacity)
        : capacity_(capacity), read_index_(0), write_index_(0)
{
    buffer_ = new char[capacity_];
}

Buffer::~Buffer()
{
    delete[] buffer_;
    capacity_ = 0;
}

// from muduo
int Buffer::ReadFd(int fd)
{
    char extrabuf[65536];
    struct iovec vec[2];
    const unsigned int writable = capacity_ - write_index_;
    vec[0].iov_base = &buffer_[write_index_];
    vec[0].iov_len = writable;
    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof(extrabuf);

    const int iovcnt = writable < sizeof(extrabuf) ? 2 : 1;

    const int n = readv(fd, vec, iovcnt);
    if (n < 0)
    {
        // TODO handle error
    }
    else if (static_cast<unsigned int>(n) <= writable)
    {
        write_index_ += n;
    }
    else // n > writable
    {
        write_index_ = capacity_;
        append(extrabuf, n - writable);
    }

    return n;
}

void Buffer::append(const void* data, unsigned int length)
{
    ensureCapacity(length);
    memcpy(&buffer_[write_index_], data, length);
    write_index_ += length;
}

void Buffer::ensureCapacity(unsigned int length)
{
    if (capacity_ - write_index_ < length)
    {
        grow(length);
    }
}

void Buffer::grow(unsigned int length)
{
    if (write_index_ + capacity_ - read_index_ < length)
    {
        unsigned int cap = capacity_ * 2 + length;
        char* buf = new char[cap];
        memcpy(buf, &buffer_[read_index_], GetLength());
        write_index_ = GetLength();
        read_index_ = 0;
        capacity_ = cap;
        delete[] buffer_;
        buffer_ = buf;
    }
    else
    {
        adjust();
    }
}

void Buffer::adjust()
{
    memmove(buffer_, &buffer_[read_index_], GetLength());
    write_index_ = GetLength();
    read_index_ = 0;
}
