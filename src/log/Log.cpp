/*
 *  Created by chrji on 2021/1/23.
 */

#include "../include/Log.h"

#include <mutex>
#include <deque>
#include <condition_variable>
#include <sys/time.h>

Log* Log::getInstance()
{
    return nullptr;
}
