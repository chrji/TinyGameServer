/*
 *  Created by chrji on 2021/1/31.
 */

#ifndef TINYGAMESERVER_BUFFER_H
#define TINYGAMESERVER_BUFFER_H

#include "../include/Base.h"

#include <string.h>

#include <vector>

#include <event2/event.h>


class Buffer
{
public:
    static const unsigned int INITIAL_SIZE = 8192;

    explicit Buffer(unsigned int capacity = INITIAL_SIZE);

    ~Buffer();

    int ReadFd(evutil_socket_t fd);


    // for read

    inline int64_t ReadInt64()
    {
        int64_t i = 0;
        memcpy(&i, &buffer_[read_index_], sizeof(i));
        read_index_ += sizeof(i);

        return ntohll(i);
    }

    inline int32_t ReadInt32()
    {
        int32_t i = 0;
        memcpy(&i, &buffer_[read_index_], sizeof(i));
        read_index_ += sizeof(i);

        return ntohl(i);
    }

    inline int16_t ReadInt16()
    {
        int16_t i = 0;
        memcpy(&i, &buffer_[read_index_], sizeof(i));
        read_index_ += sizeof(i);

        return ntohs(i);
    }

    inline int8_t ReadInt8()
    {
        int8_t i = 0;
        memcpy(&i, &buffer_[read_index_], sizeof(i));
        read_index_ += sizeof(i);

        return i;
    }

    inline std::string ReadString(unsigned int length)
    {
        if (length > GetLength())
        {
            length = GetLength();
        }
        std::string str(GetData(), length);
        read_index_ += length;

        return str;
    }

    std::pair<char, int32_t> PeekTagLen() const
    {
        char tag = buffer_[read_index_];
        int32_t len = 0;
        memcpy(&len, &buffer_[read_index_ + 1], sizeof(len));

        return std::make_pair(tag, len);
    }

    const char* GetData() const
    {
        return &buffer_[read_index_];
    }

    void Forward(int num)
    {
        if (num < GetLength())
        {
            read_index_ += num;
        }
        else
        {
            read_index_ = 0;
            write_index_ = 0;
        }
    }

    // for write

    void AppendInt64(int64_t data)
    {
        int64_t i = htonll(data);
        append(&i, sizeof(i));
    }

    void AppendInt32(int32_t data)
    {
        int32_t i = htonl(data);
        append(&i, sizeof(i));
    }

    void AppendInt16(int16_t data)
    {
        int16_t i = htons(data);
        append(&i, sizeof(i));
    }

    void AppendInt8(int8_t data)
    {
        append(&data, sizeof(data));
    }

    void AppendString(const std::string& data)
    {
        append(data.c_str(), data.length());
    }

    void AppendBuffer(const std::shared_ptr<Buffer>& data)
    {
        append(data->GetData(), data->GetLength());
    }

    void AppendData(const void* data, unsigned int length)
    {
        append(data, length);
    }

    char* WriteBegin()
    {
        return buffer_ + write_index_;
    }

    const char* WriteBegin() const
    {
        return buffer_ + write_index_;
    }


    unsigned int GetLength() const
    {
        return write_index_ - read_index_;
    }

    void Clear()
    {
        read_index_ = 0;
        write_index_ = 0;
    }

protected:

private:

    // for uint64 hton/ntoh
    uint64_t ntohll(uint64_t _netlonglong)
    {
        return (((uint64_t) htonl(_netlonglong)) << 32) + htonl(_netlonglong >> 32);
    }

    uint64_t htonll(u_int64_t _hostlonglong)
    {
        return ntohll(_hostlonglong);
    }


    void append(const void* data, unsigned int length);

    void ensureCapacity(unsigned int length);

    void grow(unsigned int length);

    void adjust();

    char* buffer_;
    unsigned int capacity_;
    unsigned int read_index_;
    unsigned int write_index_;

};


#endif //TINYGAMESERVER_BUFFER_H
