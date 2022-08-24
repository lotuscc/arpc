#include <functional>
#include <stdio.h>
#include <ucontext.h>
#include <unistd.h>
class Alligator {
public:
    static void Speak() {
        printf("YAWWW \n");
    }
    typedef void (*AlligatorSpeakFn)(void);

    void SpeakUsing(AlligatorSpeakFn fn) {
        fn();
    }
};

std::function<void(void)> g_callback;

void g_print_hello(void) {
    printf("Hello \n");

    g_callback();
}

class Connector {
    char stack[1024];

public:
    ucontext_t context;

    using callback = std::function<void(void)>;

    static callback static_call;

    Connector(int n) {
        if (getcontext(&context) == -1) {
            printf("error \n");
        }
        context.uc_stack.ss_sp = stack;
        // 栈大小
        context.uc_stack.ss_size = sizeof(stack);
        // 不指定下一个上下文
        context.uc_link = NULL;

        std::function<void(void)> call =
            std::bind(&Connector::print_hello, this, n);

        // call();

        g_callback = call;

        // auto x = static_call.target<void (*)(void)>();

        // *x = &g_print_hello;

        // call();

        // printf("debug \n");
        makecontext(&context, g_print_hello, 0);
        // makecontext(&context, *static_call.target<void (*)(void)>(), 0);
    }

    void print_hello(int n) {
        for (int i = 0; i < n; ++i) {
            printf("Hello \n");
        }
    }
};

int main(void) {
    // ucontext_t context;
    // char stack[1024];
    // if (getcontext(&context) == -1) {
    // }
    // // 初始化栈顶指针
    // context.uc_stack.ss_sp = stack;
    // // 栈大小
    // context.uc_stack.ss_size = sizeof(stack);
    // // 不指定下一个上下文
    // context.uc_link = NULL;
    // // 指定待执行的函数入口，并且该函数不需要参数
    // makecontext(&context, g_print_hello, 0);
    // setcontext(&context);

    Connector Conn(10);
    // printf("Hello \n");
    setcontext(&Conn.context);

    return 0;
}