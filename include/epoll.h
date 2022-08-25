#include <cstdio>
#include <cstdlib>
#include <sys/epoll.h>

#include "Setting.h"

#include "task.h"
#include "ts_pool.h"

class Poller {

public:
    int epollfd_;
    Setting *setting_;
    ell_ts_pool *pool_;

#define MAX_EVENTS 16

    struct epoll_event events_[MAX_EVENTS];

    Poller(Setting *setting, ell_ts_pool *pool)
        : epollfd_(::epoll_create1(EPOLL_CLOEXEC)), setting_(setting),
          pool_(pool) {}

    void append(int fd, int events) {
        struct epoll_event event;

        event.events = events;
        event.data.fd = fd;

        // if (setting_->Conns.count(fd)) {
        //     if (::epoll_ctl(epollfd_, EPOLL_CTL_MOD, fd, &event) < 0) {
        //         printf("epoll ctl failure! \n");
        //     }
        // } else {
        // setting_->Conns[fd] =

        if (::epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &event) < 0) {
            printf("epoll ctl failure!");
        }
        // }
    }

    void poll(void) {
        while (1) {
            int nfds = epoll_wait(epollfd_, events_, MAX_EVENTS, -1);
            // for gdb debug
            // if (-1 == nfds) {
            //     if (errno != EINTR) {
            //         continue;
            //     }
            // }

            if (nfds == -1) {
                perror("epoll_wait");
                exit(EXIT_FAILURE);
            }

            for (int n = 0; n < nfds; ++n) {
                int fd = events_[n].data.fd;

                if (events_[n].events & (EPOLLIN | EPOLLPRI)) {

                    printf("epoll %d read! \n", fd);
                    if (setting_->Conns.count(fd) == 0) {
                        printf("epoll poll error! \n");
                        exit(EXIT_FAILURE);
                    }
                    printf("push task addr %lx \n", setting_->Conns[fd]->t);                   
                    
                    pool_->submit(setting_->Conns[fd]->t);
                }
            }
        }
    }
};