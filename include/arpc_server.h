#pragma once

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <map>
#include <sys/ucontext.h>
#include <thread>
#include <ucontext.h>
#include <unistd.h>
#include <vector>

#include "Setting.h"
#include "arpc_connector.h"
#include "arpc_epoll.h"
#include "arpc_task.h"
#include "socket.h"
#include "ts_pool.h"


class Server {
public:
    ell_ts_pool *pool;
    Connector *acceptor;
    Setting *setting;
    Poller poller;

    // using MCallBack = std::function<void(Connector *conn, Setting *setting)>;
    // MCallBack message;

    using MessageCall = std::function<void(Connector *conn)>;
    MessageCall message;

    Server(ell_Ipv4Addr &localAddr);

    // void default_Message(Connector *conn, Setting *setting) {
    // void default_Message(Connector *conn) {}

    void defaultClose(Connector *conn, Setting *setting);
    void setdefaultMessage(MessageCall call);
    void defaultAccept(Connector *conn, Setting *setting);
    void acceptInit();
    void start();
};