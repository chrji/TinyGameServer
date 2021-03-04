/*
 * Created by chrji on 2021/3/1.
 */

#include <signal.h>

#include <iostream>

#include "include/EventLoop.h"
#include "include/Log.h"
#include "network/TCPServer.h"
#include "network/IConnection.h"
#include "util/Buffer.h"
#include "data/RedisManager.h"
#include "data/PGManager.h"

Log* Log::l_ = new Log("./Data.log");

int main()
{
    signal(SIGPIPE, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    auto loop = std::make_shared<EventLoop>();
    loop->Init();

    int conn_num = 2;
    PGManager postgres("0.0.0.0:5432", conn_num);
    postgres.Connect("george");

    RedisManager redis("0.0.0.0:6379", conn_num);
    redis.Connect("george");


    auto message_cb = [&](ConnSPtr conn) -> int
    {

        auto connBuf = conn->GetBuffer();
        {
            if (connBuf->GetLength() >= 13)
            {
                auto taglen = connBuf->PeekTagLen();
                if (taglen.second <= connBuf->GetLength() - 1)
                {
                    switch (connBuf->ReadInt8())
                    {
                        // for login
                        // | l | len |  id  | len |  name   |  len   |  password  |
                        //   1    4      8     4       -        4           -
                        case 'l':
                        {
                            connBuf->ReadInt32();
                            int64_t id = connBuf->ReadInt64();

                            uint32_t name_len = connBuf->ReadInt32();
                            std::string name = connBuf->ReadString(name_len);

                            uint32_t pass_len = connBuf->ReadInt32();
                            std::string pass = connBuf->ReadString(pass_len);

                            int64_t uid = postgres.SelectUser(name, pass);

                            auto buf = std::make_shared<Buffer>();
                            if (uid == -1) // login error
                            {
                                // | l | len |  id  |
                                buf->AppendInt8('x');
                                buf->AppendInt32(12);
                                buf->AppendInt64(id);
                                conn->Send(buf);
                            }
                            else // login success
                            {
                                // | l | len |  id  |  uid  |
                                buf->AppendInt8('s');
                                buf->AppendInt32(20);
                                buf->AppendInt64(id);
                                buf->AppendInt64(uid);
                                conn->Send(buf);
                            }
                            break;
                        }
                        default: // other tag
                        {
                            unsigned int len = connBuf->ReadInt32();
                            connBuf->Forward(len);
                            return READ_TYPE_ERROR;
                        }
                    }
                }
                else
                {
                    return READ_INCOMPLETE;
                }
            }
            else
            {
                return READ_INCOMPLETE;
            }
        }
        return READ_SUCCESS;
    };

    std::string addr = "0.0.0.0:10241";
    int thread_num = 2;
    TCPServer server(loop, addr, thread_num);
    server.SetMessageCallBack(message_cb);
    server.Start();

    loop->Run();

    return 0;
}