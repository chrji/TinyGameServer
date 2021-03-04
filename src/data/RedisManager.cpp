/*
 *  Created by chrji on 2021/1/19.
 */

#include "RedisManager.h"

#include <string.h>
#include <assert.h>

#include <vector>

RedisManager::RedisManager(const std::string& addr, unsigned int num)
        : addr_(addr), conn_num_(num)
{
}

RedisManager::~RedisManager()
{
    for (auto& c : conns_)
    {
        redisFree(c);
    }
}

bool RedisManager::Connect(const std::string& password)
{
    auto host_port = SplitHostPort(addr_);

    for (int i = 0; i < conn_num_; i++)
    {
        auto c = redisConnect(host_port.first.c_str(), host_port.second);
        if (c != nullptr && c->err == 0)
        {
            auto reply = (redisReply*) redisCommand(c, "AUTH %s", password.c_str());
            if (reply != nullptr)
            {
                if (reply->type != REDIS_REPLY_ERROR)
                {
                    conns_.push_back(c);
                }
                freeReplyObject(reply);
            }
        }
        else if (c != nullptr && c->err != 0)
        {
            redisFree(c);
        }
    }

    assert(!conns_.empty());

    return true;
}

bool RedisManager::Set(std::string key, std::string value)
{
    auto c = PickContext();
    bool ret = false;

    auto reply = (redisReply*) redisCommand(c, "SET %s %s", key.c_str(), value.c_str());
    if (reply != nullptr)
    {
        if (reply->type != REDIS_REPLY_ERROR)
        {
            ret = true;
        }
        freeReplyObject(reply);
    }

    return ret;
}

bool RedisManager::Get(std::string key, std::string& value)
{
    auto c = PickContext();
    bool ret = false;

    auto reply = (redisReply*) redisCommand(c, "GET %s", key.c_str());
    if (reply != nullptr)
    {
        if (reply->type == REDIS_REPLY_NIL)
        {
            ret = true;
        }
        else if (reply->type != REDIS_REPLY_ERROR && reply->type != REDIS_REPLY_NIL)
        {
            ret = true;
            value = reply->str;
        }
        freeReplyObject(reply);
    }

    return ret;
}

redisContext* RedisManager::PickContext()
{
    static int i = 0;
    redisContext* c = nullptr;
    if (!conns_.empty())
    {
        c = conns_[i++ % conns_.size()];
    }
    return c;
}
