#include <cstdlib>
#include <stdint.h>
#include <stdio.h>
#include <ucontext.h>
#include <unistd.h>
#include <string.h>

struct schedule {
    int id;
    int cap;
    int running;
};

static void mainfunc(uint32_t low32, uint32_t hi32) {
    uintptr_t ptr = (uintptr_t)low32 | ((uintptr_t)hi32 << 32);
    struct schedule *S = (struct schedule *)ptr;

    printf("Hello id: %d \n", S->id);
}

int main(void) {
    ucontext_t context;
    char stack[1024];
    if (getcontext(&context) == -1) {
    }
    // 初始化栈顶指针
    context.uc_stack.ss_sp = stack;
    // 栈大小
    context.uc_stack.ss_size = sizeof(stack);
    // 不指定下一个上下文
    context.uc_link = NULL;
    // 指定待执行的函数入口，并且该函数不需要参数
    
    auto S = (struct schedule*)malloc(sizeof(struct schedule));

    S->id = 717;

    uintptr_t ptr = (uintptr_t)S;

    makecontext(&context, (void (*)(void))mainfunc, 2, (uint32_t)ptr,
                (uint32_t)(ptr >> 32));

    setcontext(&context);
    return 0;
}