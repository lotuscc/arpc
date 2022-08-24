#pragma once

#include "Setting.h"
#include "connection.h"
#include "socket.h"
#include "task.h"
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <functional>
#include <map>
#include <ucontext.h>
#include <unistd.h>
#include <vector>

#include "epoll.h"
#include <thread>

#include "ts_pool.h"

// int g_size = 0;

void g_callback_start(uint32_t low32, uint32_t hi32) {
    // printf("g_message_start \n");
    // printf("enter g_callback_start %d \n", ++g_size);
    uintptr_t ptr = (uintptr_t)low32 | ((uintptr_t)hi32 << 32);

    Connector *S = (Connector *)ptr;
    S->callback();

    // 处理完任务之后返回
    // setcontext(&S->t->parent);

    while (1) {
        printf("exit \n");
        sleep(1);
    }
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
        acceptor->socket.listen();
    }

    void Message(Connector *conn, Setting *setting) {
        char data[64];

        while (1) {
            memset(data, '\0', 64);
            conn->read_nbytes(data, 64);

            // conn->read_until(data, 4);

            printf("read %d bytes:\n%s \n", 64, data);

            conn->send_nbytes(data, 64);
            // conn->send_nbytes((void *)"hello", 5);
        }

        // conn->resume();

        // while (1) {
        //     sleep(1);
        //     printf("wait \n");
        // }
    }
    void Accept(Connector *conn, Setting *setting) {
        while (1) {
            auto client_addr = new ell_Ipv4Addr();

            // accept 后期改成异步
            // 直接accept 非阻塞
            int connfd = ::accept4(conn->socket.fd(), client_addr->addr(),
                                   client_addr->len_addr(), SOCK_NONBLOCK);

            printf("accept: %d \n", connfd);

            Connector *client_conn = new Connector(connfd, *client_addr);

            client_conn->callback =
                std::bind(&Server::Message, this, client_conn, setting);

            uintptr_t ptr = (uintptr_t)client_conn;

            client_conn->t = new task((void (*)(void))g_callback_start, ptr);

            setting->Conns[connfd] = client_conn;

            poller.append(connfd, EPOLLIN | EPOLLET);

            printf("in start,  client_conn addr : %lx \n",
                   (uintptr_t)client_conn);

            // 主动 yield
            conn->t->yield();
        }
    }

    void acceptInit() {
        acceptor->callback =
            std::bind(&Server::Accept, this, acceptor, setting);

        acceptor->t = new task((void (*)(void))g_callback_start, (uintptr_t)acceptor);

        setting->Conns[acceptor->socket.fd()] = acceptor;

        poller.append(acceptor->socket.fd(), EPOLLIN | EPOLLET);
    }

    // void accept() {

    //     std::thread t(&Server::start, this);

    //     // t.detach();

    //     while (1) {
    //         auto client_addr = new ell_Ipv4Addr();

    //         // accept 后期改成异步
    //         // 直接accept 非阻塞
    //         int connfd = ::accept4(acceptor->socket.fd(), client_addr->addr(),
    //                                client_addr->len_addr(), SOCK_NONBLOCK);

    //         printf("accept: %d \n", connfd);

    //         Connector *client_conn = new Connector(connfd, *client_addr);

    //         client_conn->callback =
    //             std::bind(&Server::Message, this, client_conn, setting);

    //         uintptr_t ptr = (uintptr_t)client_conn;

    //         client_conn->t = new task((void (*)(void))g_callback_start, ptr);

    //         setting->Conns[connfd] = client_conn;

    //         poller.append(connfd, EPOLLIN | EPOLLET);

    //         printf("in start,  client_conn addr : %lx \n",
    //                (uintptr_t)client_conn);

    //         // 取消注释会多运行一次
    //         // task_queue.push(client_conn->t);
    //     }
    // }

    void start() {

        acceptInit();

        poller.poll();
    }
};