/*
 *  Created by chrji on 2021/1/23.
 */

#include "include/Log.h"


#include <stdarg.h>
#include <sys/time.h>

#include <mutex>
#include <deque>
#include <condition_variable>


Log::Log(std::string file) : file_(file)
{

}

Log::~Log()
{

}

void Log::log(LogType type, const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    // TODO

    va_end(ap);
}
