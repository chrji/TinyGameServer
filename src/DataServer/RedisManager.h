/*
 *  Created by chrji on 2021/1/19.
 */

#ifndef TINYGAMESERVER_REDISMANAGER_H
#define TINYGAMESERVER_REDISMANAGER_H

#include <string>
#include <atomic>

#include <hiredis/hiredis.h>

#include "../include/Base.h"

class RedisManager
{
public:
    explicit RedisManager(const std::string& addr, unsigned int num = 2);

    ~RedisManager();

    bool Connect(const std::string& password);

    bool Set(std::string key, std::string value);

    bool Get(std::string key, std::string& value);


protected:

private:
    redisContext* PickContext();


    std::string addr_;
    std::atomic<ConnStatus> status_;

    std::mutex mutex_conns_;
    std::vector<redisContext*> conns_;
    int conn_num_;

};


#endif //TINYGAMESERVER_REDISMANAGER_H
