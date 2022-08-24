#pragma once

// #ifndef ELL_SOCKET_H
// #define ELL_SOCKET_H

#include <cassert>
#include <cerrno>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <memory>
#include <sys/socket.h>

#include "IPv4Addr.h"
#include "log.h"
#include "socketOps.h"
#include "socket.h"

void ell_Socket::remake(int fd) { sockfd_ = fd; }

// 创建默认socket IPv4、非阻塞
int ell_Socket::create_defaultsocket() {
    // SOCK_NONBLOCK
    return sockops::socket(PF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
}

ell_Socket::ell_Socket() : sockfd_(create_defaultsocket()) {}

ell_Socket::ell_Socket(int fd) : sockfd_(fd) {}

ell_Socket::~ell_Socket() {}

void ell_Socket::connection(ell_Ipv4Addr &serverAddr) {
    sockops::connect(sockfd_, serverAddr.addr(), serverAddr.len());
}

void ell_Socket::bind(ell_Ipv4Addr &localaddr) {
    sockops::bind(sockfd_, localaddr.addr(), localaddr.len());
}

void ell_Socket::listen() { sockops::listen(sockfd_, 5); }

int ell_Socket::fd() const { return sockfd_; }

void ell_Socket::send(const void *__buf, size_t __n) {
    int ret = ::send(sockfd_, __buf, __n, 0);
    if (ret == -1) {
        LOG("send error!");
    }
}

void ell_Socket::recv(void *__buf, size_t __n) {
    ::recv(sockfd_, __buf, __n, 0);
}

void ell_Socket::send_to(int fd, const void *buf, size_t n) {
    ::send(fd, buf, n, 0);
}

void ell_Socket::recv_from(int fd, void *__buf, size_t __n) {
    ::recv(fd, __buf, __n, 0);
}

void ell_Socket::connection_to(int __fd, const struct sockaddr *__addr,
                               socklen_t __len) {
    sockops::connect(__fd, __addr, __len);
}

// #endif ELL_SOCKET_H