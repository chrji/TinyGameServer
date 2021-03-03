/*
 * Created by chrji on 2021/3/1.
 */

#include "../include/EventLoop.h"
#include "../network/TCPServer.h"
#include "../network/TCPClient.h"
#include "../network/IConnection.h"
#include "../util/Buffer.h"

#include "RedisManager.h"
#include "PGManager.h"

#include <iostream>

int main()
{
    std::string addr = "0.0.0.0:10240";
    int thread_num = 2;
    auto loop = std::make_shared<EventLoop>();
    loop->Init();


    auto message_cb = [&](ConnSPtr conn) -> int
    {
        static int i = 0;
        if (i++ < 10)
        {
            auto buf = std::make_shared<Buffer>();
            auto str = conn->GetBuffer()->ReadString(100);
            buf->AppendString(str);

            std::cout << conn->GetId() << ":recv:" << str << std::endl;

            auto s = std::make_shared<Buffer>();
            s->AppendString("send info");

            conn->Send(s);
        }
        return i;
    };


    TCPServer server(loop, addr, thread_num);
    server.SetMessageCallBack(message_cb);
    server.Start();

    TCPClient client(loop, "127.0.0.1:10240");
    client.SetMessageCallBack(message_cb);

    client.SetConnectionCallBack([](ConnSPtr conn) -> int
                                 {
                                     auto buf = std::make_shared<Buffer>();
                                     buf->AppendString("hello");
                                     conn->Send(buf);
                                     return 0;
                                 });
    client.Connect();

    RedisManager r("0.0.0.0:6379", 1);
    r.Connect("-std=c++11");

    r.Set("vvvv", "5");
    std::string value;
    std::cout << r.Get("vv", value) << std::endl;
    std::cout << value << std::endl;

    PGManager p("0.0.0.0:5432", 1);


    loop->Run();

    return 0;
}