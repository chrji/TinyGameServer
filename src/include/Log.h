/*
 *  Created by chrji on 2021/1/23.
 */

#ifndef TINYGAMESERVER_LOG_H
#define TINYGAMESERVER_LOG_H

#include <string>

class Log
{
public:
    static Log* getInstance();

private:
    Log();

    Log(const Log&) = delete;

    Log(Log&&) = delete;

    Log& operator=(const Log&) = delete;

    Log& operator=(Log&&) = delete;

    ~Log();

    Log* l;

};


#endif //TINYGAMESERVER_LOG_H
