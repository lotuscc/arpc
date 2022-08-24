#include <stdio.h>
#include <ucontext.h>


ucontext_t ctx[2];

void hello(void) {

    printf("start hello world\n");
    // 将当前 context 保存到 ctx[1]，切换到 ctx[0] 执行 return 0;
    swapcontext(&ctx[1], &ctx[0]);
    printf("finish hello world\n");

    printf("%d", 99);

    swapcontext(&ctx[1], &ctx[0]);
}

int main(int argc, char *argv[]) {
    char stack1[16 * 1024];
    if (getcontext(&ctx[1]) == -1) {
        
    }
    ctx[1].uc_stack.ss_sp = stack1;
    ctx[1].uc_stack.ss_size = sizeof(stack1);
    ctx[1].uc_link = NULL;

    makecontext(&ctx[1], hello, 0);

    printf("before\n");
    // 将当前 context 保存到 ctx[0]，切换到 ctx[1]
    swapcontext(&ctx[0], &ctx[1]);
    
    printf("over %d", 44);
    return 0;
}