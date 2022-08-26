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

// 有个bug未修复
// 同时只能有一个线程获取task的执行权
// 如果同时有多个线程正在执行同task，
// 也就是上一个context还未保存，此时另一个线程又开始执行context，
// 会造成一段代码被重复执行

// 解决办法，可以在task中设置一个原子状态位，显示其状态，

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
    running = true;
    swapcontext(&parent, &context);
}
void task::yield() {
    running = false;
    swapcontext(&context, &parent);
}
