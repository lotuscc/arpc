#pragma once

#include <cstddef>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>

#include "log.h"

class sockops {
public:
    inline static int socket(int __domain, int __type, int __protocol) {
        auto ret = ::socket(__domain, __type, __protocol);
        if (ret == -1) {
            LOG("create socket failture!");
            LOG("errON: {} errMsg: {}", errno, strerror(errno));
        }
        return ret;
    }
    inline static int bind(int __fd, const struct sockaddr *__addr,
                           socklen_t __len) {
        auto ret = ::bind(__fd, __addr, __len);
        if (ret == -1) {
            LOG("bind socket failture!");
            LOG("errON: {} errMsg: {}", errno, strerror(errno));
        }
        return ret;
    }
    inline static int listen(int __fd, int __n) {
        auto ret = ::listen(__fd, __n);
        if (ret == -1) {
            LOG("listen socket failture!");
            LOG("errON: {} errMsg: {}", errno, strerror(errno));
        }
        return ret;
    }
    inline static int accept(int __fd, struct sockaddr *__restrict __addr,
                             socklen_t *__restrict __addr_len) {
        auto ret = ::accept(__fd, __addr, __addr_len);
        if (ret == -1) {
            LOG("accept socket failture!");
            LOG("errON: {} errMsg: {}", errno, strerror(errno));
        }
        return ret;
    }
    inline static int connect(int __fd, const struct sockaddr *__addr,
                              socklen_t __len) {
        auto ret = ::connect(__fd, __addr, __len);
        if (ret == -1) {
            LOG("connect socket failture!");
            LOG("errON: {} errMsg: {}", errno, strerror(errno));
        }
        return ret;
    }
    inline static int close(int __fd) {
        auto ret = ::close(__fd);
        if (ret == -1) {
            LOG("close socket failture!");
            LOG("errON: {} errMsg: {}", errno, strerror(errno));
        }
        return ret;
    }
    inline static int shutdown(int __fd, int __how) {
        auto ret = ::shutdown(__fd, __how);
        if (ret == -1) {
            LOG("shutdown socket failture!");
            LOG("errON: {} errMsg: {}", errno, strerror(errno));
        }
        return ret;
    }

    inline static size_t send(int __fd, const void *__buf, size_t __n,
                              int __flags) {
        auto ret = ::send(__fd, __buf, __n, __flags);
        if (ret == -1) {
            LOG("send socket failture!");
            LOG("errON: {} errMsg: {}", errno, strerror(errno));
        }
        return ret;
    }
    inline static size_t recv(int __fd, void *__buf, size_t __n, int __flags) {
        auto ret = ::recv(__fd, __buf, __n, __flags);
        if (ret == -1) {
            LOG("recv socket failture!");
            LOG("errON: {} errMsg: {}", errno, strerror(errno));
        }
        return ret;
    }
};
