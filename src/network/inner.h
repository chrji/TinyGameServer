/*
 * Created by chrji on 2021/3/1.
 */

#ifndef TINYGAMESERVER_INNER_H
#define TINYGAMESERVER_INNER_H

#include <memory>

class EventLoop;

class IAcceptor;

class IConnector;

class IConnection;


typedef std::shared_ptr<EventLoop> LoopSPtr;

typedef std::shared_ptr<IConnection> ConnSPtr;

#endif //TINYGAMESERVER_INNER_H
