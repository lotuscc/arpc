#pragma once

#include "Setting.h"
#include "arpc_connector.h"
#include "socket.h"
#include "task.h"
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <map>
#include <sys/ucontext.h>
#include <ucontext.h>
#include <unistd.h>
#include <vector>

#include "epoll.h"
#include <thread>

#include "ts_pool.h"

// int g_size = 0;

ucontext_t g_parent;

void g_callback_start(uint32_t low32, uint32_t hi32) {
    printf("g_message_start \n");
    // printf("enter g_callback_start %d \n", ++g_size);
    uintptr_t ptr = (uintptr_t)low32 | ((uintptr_t)hi32 << 32);

    Connector *Conn = (Connector *)ptr;

    // printf("task %lx \n", &S->t->parent);

    Conn->ReadCallBack();

    // 处理完任务之后返回
    g_parent = Conn->t->parent;

    Conn->CloseCallBack();   
    // ucontext_t* p = (ucontext_t*)malloc(sizeof(ucontext_t));

    // 释放Conn
    // free(Conn)

    setcontext(&g_parent);

    // while (1) {
    //     printf("exit \n");
    //     sleep(1);
    // }
}

class Server {
public:
    ell_ts_pool *pool;
    Connector *acceptor;
    Setting *setting;
    Poller poller;

    // using MCallBack = std::function<void(Connector *conn, Setting *setting)>;
    // MCallBack message;

    Server(ell_Ipv4Addr &localAddr)
        : pool(new ell_ts_pool(1)), acceptor(new Connector(localAddr)),
          setting(new Setting()), poller(setting, pool) {
        acceptor->socket.bind(localAddr);

        acceptInit();
    }

    // void default_Message(Connector *conn, Setting *setting) {
    // void default_Message(Connector *conn) {}

    void default_Close(Connector *conn, Setting *setting) {
        conn->close();

        // 从Conns中去除
        setting->Conns.erase(conn->socket.fd());
    }

    using MessageCall = std::function<void(Connector *conn)>;

    MessageCall message;

    void setdefaultMessage(MessageCall call) {
        message = std::move(call);
    }

    void Accept(Connector *conn, Setting *setting) {
        while (1) {
            auto client_addr = new ell_Ipv4Addr();

            // accept 后期改成异步
            // 直接accept 非阻塞

            printf("start accept4 \n");
            int ret = ::accept4(conn->socket.fd(), client_addr->addr(),
                                client_addr->len_addr(), SOCK_NONBLOCK);

            if (ret == EAGAIN || ret == EWOULDBLOCK) {
                printf("no new connection! ");
                conn->t->yield();

                continue;
            } else if (ret == -1) {
                printf("errON: %d errMsg: %s \n", errno, strerror(errno));
                conn->t->yield();

                continue;
            }

            int connfd = ret;

            printf("accept: %d \n", connfd);

            Connector *client_conn = new Connector(connfd, *client_addr);

            // 这句必须加上，否则clang 编译器会出现奇怪的bug
            // Connector *copy = client_conn;

            // gcc clang 编译器行为不同
            // 此处有个bug

            // client_conn->callback =
            //     std::bind(&Server::default_Message, this, copy);

            client_conn->ReadCallBack = std::bind(message, client_conn);

            client_conn->CloseCallBack = std::bind(&Server::default_Close, this, client_conn, setting);

            uintptr_t ptr = (uintptr_t)client_conn;
            client_conn->t = new task((void (*)(void))g_callback_start, ptr);

            // setting 为共享数据，多线程操作需要加锁
            setting->Conns[connfd] = client_conn;

            // EPOLLET 边沿触发模式
            poller.append(connfd, EPOLLIN | EPOLLET);

            printf("in start,  client_conn addr : %lx \n",
                   (uintptr_t)client_conn);

            // 主动 yield
            conn->t->yield();
        }
    }

    void acceptInit() {
        acceptor->socket.listen();

        Connector *copy = acceptor;
        acceptor->ReadCallBack =
            std::bind(&Server::Accept, this, copy, setting);

        uintptr_t ptr = (uintptr_t)acceptor;
        acceptor->t = new task((void (*)(void))g_callback_start, ptr);

        setting->Conns[acceptor->socket.fd()] = acceptor;

        poller.append(acceptor->socket.fd(), EPOLLIN | EPOLLET);
    }

    void start() {
        poller.poll();
    }
};