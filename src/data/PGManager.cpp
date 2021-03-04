/*
 *  Created by chrji on 2021/1/19.
 */

#include "PGManager.h"

#include <assert.h>

PGManager::PGManager(const std::string& addr, unsigned int num)
        : addr_(addr), conn_num_(num)
{
}

PGManager::~PGManager()
{
    for (auto& c : conns_)
    {
        PQfinish(c);
    }
}


bool PGManager::Connect(const std::string& password)
{
    auto host_port = SplitHostPort(addr_);

    char conninfo[128] = {};
    sprintf(conninfo, "hostaddr=%s port=%d dbname=%s user=%s password=%s",
            host_port.first.c_str(), host_port.second, "postgres", "postgres", password.c_str());

    for (int i = 0; i < conn_num_; i++)
    {
        auto c = PQconnectdb(conninfo);
        if (c != nullptr)
        {
            if (PQstatus(c) == CONNECTION_OK)
            {
                conns_.push_back(c);
            }
            else
            {
                PQfinish(c);
            }
        }
    }

    assert(!conns_.empty());

    return true;
}


int64_t PGManager::SelectUser(std::string name, std::string password)
{
    auto c = PickContext();
    int64_t ret = -1;
    char sql[128] = {};
    sprintf(sql, "select uid from GameUser where username = '%s' and password = '%s';",
            name.c_str(), password.c_str());

    auto res = PQexec(c, sql);
    auto x = PQresultStatus(res);
    if (res != nullptr || PQresultStatus(res) == PGRES_COMMAND_OK || PQresultStatus(res) == PGRES_TUPLES_OK)
    {
        int rows = PQntuples(res);
        for (int i = 0; i < rows; i++)
        {
            auto c = PQgetvalue(res, i, 0);

            ret = strtoll(c, nullptr, 10);
        }

        PQclear(res);
        return ret;
    }

    PQclear(res);
    return ret;
}

PGconn* PGManager::PickContext()
{
    static int i = 0;
    PGconn* c = nullptr;
    if (!conns_.empty())
    {
        c = conns_[i++ % conns_.size()];
    }
    return c;
}
