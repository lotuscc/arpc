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

    task(void (*func)(void), uintptr_t ptr) {
        if (getcontext(&context) == -1) {
            printf("make task failture! \n");
        }
        
        // parent = context;

        ss_sp = malloc(STACK_SIZE);
        context.uc_stack.ss_sp = ss_sp;
        context.uc_stack.ss_size = STACK_SIZE;
        context.uc_link = NULL;

        makecontext(&context, func, 2, (uint32_t)ptr, (uint32_t)(ptr >> 32));
    }
    ~task() {
        if (ss_sp != NULL) {
            free(ss_sp);
            ss_sp = NULL;
        }
    }

    task(const task &) = delete;
    task &operator=(const task &) = delete;

    void resume() {
        swapcontext(&parent, &context);
    }
    void yield() {
        swapcontext(&context, &parent);
    }

    void copy(ucontext_t* ctx){
        *ctx = context;
    }

    int id;
};

// void make_task(task *t, void (*func)(void), uintptr_t ptr);

extern std::queue<task *> task_queue;