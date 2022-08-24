#include <ucontext.h>
#include<unistd.h>
#include<stdio.h>

int main() {
    ucontext_t context;

    getcontext(&context);
    printf("Hello world\n");
    sleep(1);
    setcontext(&context);
    return 0;
}