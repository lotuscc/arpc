#pragma once

#include "task.h"
#include <cstddef>
#include <cstdio>
#include <functional>
#include <string>

#include "socket.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

#include "socketOps.h"

class Connector {
private:
public:
    using EeventCallBack = std::function<void(void)>;

    task *t = NULL;

    ell_Socket socket;
    EeventCallBack ReadCallBack;

    EeventCallBack WriteCallBack;
    EeventCallBack CloseCallBack;
    EeventCallBack ErrorCallBack;

    Connector(int fd, ell_Ipv4Addr addr) : socket(fd) {
        // t = new task();
    }

    Connector(ell_Ipv4Addr addr) : socket() {
        // t = new task();
    }

    ~Connector(){
        
    }

    // std::function<void(void)> call;

    // 存在几种策略：
    // 直接读取所有数据，将多余的数据保存在缓存中，直到无数据可读 yield，
    // 只读取所需要的数据。

    void read_nbytes(void *buf, int size) {
        int cur_recved_n = 0;
        while (1) {
            if (cur_recved_n == size) {
                return;
            }
            int ret = ::recv(socket.fd(), (char *)buf + cur_recved_n,
                             size - cur_recved_n, 0);
            if (ret == 0) {
                printf("error: close  fd: %d \n", socket.fd());
                
                t->yield();

            } else if (ret < 0) {
                if (ret == EAGAIN) {
                    // 无数据可读，切换回去                    
                    t->yield();

                } else {
                    printf("other error fd: %d \n", socket.fd());

                    sockops::close(socket.fd());                    
                    t->yield();
                }
            } else if (ret > 0) {
                // 读取ret字节数据
                cur_recved_n += ret;
            }
        }
    }

    // 一次性读取socket缓存中的数据，最多读取size
    // 如果读满size立刻返回，否则直接返回，返回ret实际读取的数据量
    int read_allbytes(void *buf, int size) {
        int ret = ::recv(socket.fd(), (char *)buf, size, 0);
        if (ret == 0) {
            printf("error: close \n fd: %d", socket.fd());
        } else if (ret < 0) {
            if (ret == EAGAIN) {
                // 无数据可读，直接返回
                return ret;
            } else {
                printf("other error fd: %d", socket.fd());
            }
        } else if (ret > 0) {
            // 读取ret字节数据
            return ret;
        }
    }

    void send_nbytes(void *buf, int size) {
        sockops::send(socket.fd(), buf, size, 0);
    }

    // 如果无法一开始就确定读取的数据量，必须加缓冲 用于处理读取的多余的数据
    void read_until_Enter(void *buf, int size) {}

    void close() {        
        sockops::close(socket.fd());
    }

    void accept() {
        // int r = ::accept()
    }
};