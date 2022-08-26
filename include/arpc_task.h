#pragma once

#include <cstddef>
#include <cstdlib>
#include <queue>
#include <stdint.h>
#include <stdio.h>
#include <sys/ucontext.h>
#include <ucontext.h>
#include <vector>

// yield / resume

class task {
public:
    // using __func = void (*)(void);
    // uintptr_t data_;
    // __func func_;

#define STACK_SIZE 1024

    ucontext_t parent;
    ucontext_t context;

    void *ss_sp = NULL;
    size_t ss_size = STACK_SIZE;

    int id;

    task(void (*func)(void), uintptr_t ptr);
    ~task();

    task(const task &) = delete;
    task &operator=(const task &) = delete;

    void resume();
    void yield();
};