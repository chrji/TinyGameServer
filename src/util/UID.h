/*
 * Created by chrji on 2021/2/21.
 */

#ifndef TINYGAMESERVER_UID_H
#define TINYGAMESERVER_UID_H

#include <stdio.h>
#include <time.h>

#include <string>
#include <atomic>

class UID
{
public:
    UID() = delete;

    UID(const UID&) = delete;

    UID& operator=(const UID&) = delete;

    ~UID();

    static int64_t GenerateSerialID()
    {
        static auto current_time = time(nullptr);
        static std::atomic<int> serial(0);

        auto current = time(nullptr);

        if (current != current_time)
        {
            current_time = current;
            serial = 0;
        }

        char str_time[64] = {};
        std::strftime(str_time, 64, "%Y%m%d%H%M%S", std::localtime(&current_time));
        auto ll = std::strtoll(str_time, nullptr, 10);

        return ((int64_t)ll * 1000 + (int64_t) ++serial);
    }

protected:

private:

};

#endif //TINYGAMESERVER_UID_H
