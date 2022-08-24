#include <stdio.h>
#include <ucontext.h>
#include <unistd.h>

void print_hello(void) {
    printf("Hello \n");
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
    makecontext(&context, print_hello, 0);
    setcontext(&context);
    return 0;
}