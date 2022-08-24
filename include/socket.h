#pragma once

#include <cassert>
#include <cerrno>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <memory>
#include <sys/socket.h>

#include "IPv4Addr.h"

class ell_Socket {
private:
    int sockfd_;

public:
    void remake(int fd);

public:
    ell_Socket();
    ell_Socket(int fd);
    ~ell_Socket();

    ell_Socket(const ell_Socket &) = delete;
    ell_Socket &operator=(const ell_Socket &) = delete;

    // call by Channel ...
    int fd() const;

    // call by TcpServer
    void bind(ell_Ipv4Addr &localaddr);

    // call by TcpServer
    void listen();

    // call by TcpClient
    void connection(ell_Ipv4Addr &serverAddr);

    void send(const void *__buf, size_t __n);
    void recv(void *__buf, size_t __n);

    inline static void recv_from(int fd, void *__buf, size_t __n);
    inline static void send_to(int fd, const void *__buf, size_t __n);
    inline static void connection_to(int __fd, const struct sockaddr *__addr,
                                     socklen_t __len);

    inline static int create_defaultsocket(void);
};
