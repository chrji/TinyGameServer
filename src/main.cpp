#include <iostream>
#include <postgresql/libpq-fe.h>
#include <vector>
#include <algorithm>
#include <string>
#include <functional>
#include <thread>
#include <chrono>

#include <event.h>
#include <map>
#include <tuple>
#include <iomanip>


#include "include/Log.h"
#include "include/EventLoop.h"

#include "network/TCPClient.h"
#include "network/TCPServer.h"


int main()
{
    std::cout << "Hello, World!" << std::endl;

//    PGconn* conn = PQconnectdb("user=postgres dbname=postgres password=george");
//
//    if (PQstatus(conn) == CONNECTION_BAD)
//
//        fprintf(stderr, "Connection to database failed: %s\n",
//                PQerrorMessage(conn));

    auto loop = std::make_shared<EventLoop>();

    TCPServer server(loop, "192.168.0.1:10240");


    server.SetConnectionCallBack([](ConnSPtr conn) -> int
                                 {
//                                    conn.get()
                                     return 0;
                                 });

    server.Start();

    TCPClient client(loop, "192.168.0.1:10240");
    client.Connect();

    loop->Run();

    return 0;
}
