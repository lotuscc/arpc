#pragma once

#include <cstdio>
#include <cstdlib>
#include <sys/epoll.h>

#include "Setting.h"

#include "arpc_task.h"
#include "ts_pool.h"

class Poller {

public:
    int epollfd_;
    Setting *setting_;
    ell_ts_pool *pool_;

#define MAX_EVENTS 16

    struct epoll_event events_[MAX_EVENTS];

    Poller(Setting *setting, ell_ts_pool *pool);

    void append(int fd, int events);

    void poll(void);
};