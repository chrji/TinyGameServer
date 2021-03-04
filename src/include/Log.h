/*
 *  Created by chrji on 2021/1/23.
 */

#ifndef TINYGAMESERVER_LOG_H
#define TINYGAMESERVER_LOG_H

#include <string>

enum LogType
{
    DEBUG = 0,
    INFO = 1,
    WARN = 2,
    ERROR = 3,
    FATAL = 4
};

class Log
{
public:
    Log(const Log&) = delete;

    Log(Log&&) = delete;

    Log& operator=(const Log&) = delete;

    Log& operator=(Log&&) = delete;


    static Log* GetInstance()
    {
        return l_;
    }

    void log(LogType type, const char* fmt, ...);

private:
    Log(std::string file);

    ~Log();

    static Log* l_;

    std::string file_;

};

#define LOG_INSTACNE Log::GetInstance()

#define LOG_DEBUG(FMT, ...) do {                    \
        LOG_INSTACNE->log(DEBUG ,"[%s:%s:%d]: "#FMT, __FILE__, __FUNCTION__,__LINE__, ##__VA_ARGS__); \
    } while (0)


#define LOG_INFO(FMT, ...) do {                        \
        LOG_INSTACNE->log(INFO,"[%s:%s:%d]: "#FMT, __FILE__, __FUNCTION__,__LINE__, ##__VA_ARGS__); \
    } while (0)


#define LOG_WARN(FMT, ...) do {                        \
        LOG_INSTACNE->log(WARN,"[%s:%s:%d]: "#FMT, __FILE__, __FUNCTION__ ,__LINE__, ##__VA_ARGS__); \
    } while (0)


#define LOG_ERROR(FMT, ...) do {                        \
        LOG_INSTACNE->log(ERROR,"[%s:%s:%d]: "#FMT, __FILE__, __FUNCTION__,__LINE__, ##__VA_ARGS__); \
    } while (0)


#define LOG_FATAL(FMT, ...) do {                    \
        LOG_INSTACNE->log(FATAL,"[%s:%s:%d]: "#FMT, __FILE__, __FUNCTION__,__LINE__, ##__VA_ARGS__); \
    } while (0)


#endif //TINYGAMESERVER_LOG_H
