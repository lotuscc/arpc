#pragma once

#include <atomic>

#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>

#include "ts_queue.h"

#include "task.h"
#include <unistd.h>


class ell_ts_pool {
    // using work_t = function_wrapper;
    // using work_t = std::function<void()>;

    using work_t = task *;

    std::atomic_bool done;
    ts_queue<work_t> work_queue;
    std::vector<std::jthread> threads;

    void worker_thread() {

        while (!done) {
            work_t task;
            if (work_queue.try_pop(task)) {
                printf("start task \n");
                // 恢复该协程的执行
                task->resume();
                printf("from parent \n");
            } else {
                // std::this_thread::yield();

                sleep(1);
                printf("sleep \n");
            }
        }
    }

public:
    ell_ts_pool(unsigned thread_count) : done(false) {
        // unsigned const thread_count = 4;
        try {
            for (unsigned i = 0; i < thread_count; ++i) {
                threads.push_back(
                    std::jthread(&ell_ts_pool::worker_thread, this));
            }
        } catch (...) {
            done = true;
            throw;
        }
    }
    ~ell_ts_pool() {
        done = true;
    }
    void submit(work_t f) {
        work_queue.push(f);
    }

    // template <typename FunctionType>
    // void submit(FunctionType f) {
    //     work_queue.push(f);
    // }
};
