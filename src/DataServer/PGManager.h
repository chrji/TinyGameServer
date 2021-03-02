/*
 *  Created by chrji on 2021/1/19.
 */

#ifndef TINYGAMESERVER_PGMANAGER_H
#define TINYGAMESERVER_PGMANAGER_H

#include <postgresql/libpq-fe.h>

#include <string>

class PGManager
{
public:
    PGManager();

    ~PGManager();

protected:

private:

    PGconn* conn_;
    PGresult* res_;

    std::string conninfo_;

    int fields_;

};


#endif //TINYGAMESERVER_PGMANAGER_H
