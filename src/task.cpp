#include "task.h"
#include <string>
#include <ucontext.h>

#include <queue>
#include <stdint.h>
#include <vector>

#include <mutex>
#include <thread>


class task_queue {
    std::queue<task *> queue;

    
        

};

// void make_task(task *t, void (*func)(void), uintptr_t ptr) {
//     if (getcontext(&t->context) == -1) {
//         printf("make task failture! \n");
//     }
//     t->context.uc_stack.ss_sp = malloc(1024);
//     t->context.uc_stack.ss_size = 1024;
//     t->context.uc_link = NULL;

//     // t->data = data;

//     makecontext(&t->context, func, 2, (uint32_t)ptr,(uint32_t)(ptr >> 32));
// }

std::queue<task *> task_queue;