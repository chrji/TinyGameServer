/*
 *  Created by chrji on 2021/1/19.
 */

#ifndef TINYGAMESERVER_PGMANAGER_H
#define TINYGAMESERVER_PGMANAGER_H

#include <postgresql/libpq-fe.h>

#include <string>
#include <vector>
#include <atomic>

#include "../include/Base.h"

class PGManager
{
public:
    explicit PGManager(const std::string& addr, unsigned int num = 2);

    ~PGManager();

    bool Connect(const std::string& password);

    int64_t SelectUser(std::string name, std::string password);

protected:

private:
    PGconn* PickContext();


    std::string addr_;
    std::atomic<ConnStatus> status_;

    std::mutex mutex_conns_;
    std::vector<PGconn*> conns_;
    int conn_num_;

};


#endif //TINYGAMESERVER_PGMANAGER_H
