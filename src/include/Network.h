/*
 *  Created by chrji on 2020/12/31.
 */

#ifndef TINYGAMESERVER_NETWORK_H
#define TINYGAMESERVER_NETWORK_H


#include <functional>

class NetworkPrivate;

class Network
{
public:
    Network();

    ~Network();

    void Init();

    void Listen(int);

    void OnAccept(std::function<void(unsigned char, int, void*)>, void*);

    void OnRecv(std::function<void(unsigned char, int, void*)>, void*);

    void OnClose(std::function<void(unsigned char, int, void*)>, void*);

private:
    NetworkPrivate* p;
};

#endif //TINYGAMESERVER_NETWORK_H
