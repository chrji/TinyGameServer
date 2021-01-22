#include <iostream>
#include <postgresql/libpq-fe.h>
#include <vector>
#include <algorithm>

#include <event.h>

#include "include/network.h"


int main()
{
    std::cout << "Hello, World!" << std::endl;

    PGconn *conn = PQconnectdb("user=postgres dbname=postgres password=george");

    if (PQstatus(conn) == CONNECTION_BAD)

        fprintf(stderr, "Connection to database failed: %s\n",
                PQerrorMessage(conn));

    EV_FEATURE_ET;
    struct event_base* base = event_base_new();



//    struct event *ev_sockfd = event_new(base, sockfd,
//                                        EV_READ | EV_PERSIST,
//                                        socket_read_cb, NULL);


    return 0;
}
