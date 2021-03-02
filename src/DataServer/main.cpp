/*
 * Created by chrji on 2021/3/1.
 */

#include "../include/EventLoop.h"
#include "../network/TCPServer.h"
#include "../network/TCPClient.h"
#include "../network/IConnection.h"
#include "../util/Buffer.h"

#include <iostream>

int main()
{
    std::string addr = "0.0.0.0:10240";
    int thread_num = 4;
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

    loop->Run();

    return 0;
}