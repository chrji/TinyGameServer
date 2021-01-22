//
// Created by chrji on 2020/12/31.
//

#ifndef TINYGAMESERVER_NETWORK_H
#define TINYGAMESERVER_NETWORK_H

#include <event.h>

class P_Network;

class Network
{
public:
    Network();
    ~Network();

private:
    P_Network *net;
};

#endif //TINYGAMESERVER_NETWORK_H
