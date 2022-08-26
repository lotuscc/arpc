#pragma once

#include <cstddef>
#include <cstdlib>
#include <queue>
#include <stdint.h>
#include <stdio.h>
#include <sys/ucontext.h>
#include <ucontext.h>
#include <vector>

#include "arpc_task.h"

// yield / resume

task::task(void (*func)(void), uintptr_t ptr) {
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
task::~task() {
    if (ss_sp != NULL) {
        free(ss_sp);
        ss_sp = NULL;
    }
}

void task::resume() {
    swapcontext(&parent, &context);
}
void task::yield() {
    swapcontext(&context, &parent);
}
