#include <cstddef>
#include <cstdlib>
#include <queue>
#include <stdio.h>
#include <string>
#include <sys/types.h>
#include <ucontext.h>
#include <unistd.h>
#include <vector>
#include <map>

void print_hello() {
    printf("Hello \n");
}

struct task {
    using __func = void (*)(void);
    void *data;
    __func func;
    ucontext_t parent;
    ucontext_t context;
};

void make_task(task *t, void (*func)(void), void *data) {
    if (getcontext(&t->context) == -1) {
        printf("make task failture! \n");
    }
    t->context.uc_stack.ss_sp = malloc(1024);
    t->context.uc_stack.ss_size = 1024;
    t->context.uc_link = NULL;

    t->data = data;

    makecontext(&t->context, func, 0);
}

std::queue<task *> task_queue;

std::vector<task *> task_wait_list;

void epoll_wait() {

    // 根据结果产生task
    // 如果fd已经存在说明为一个未完成的task，此时继续读取数，如果不存在说明为新任务

    // if(fd in task_wait){
    // do_read(99);
    // }else{  new Connection
    // do_accept(99);
    // }
}

std::map<int, task*> fd_map;

void do_accept(int fd) {
    task *t = (task *)malloc(sizeof(task));

    make_task(t, print_hello, NULL);

    task_queue.push(t);
}

void do_read(int fd) {

    task *t = task_wait_list[0];
    task_wait_list.pop_back();

    task_queue.push(t);
}

void handread(){
    // get fd
    int fd = 0;

    if(fd_map.count(fd)){
        do_read(99);
    }else {
        do_accept(fd);
    }
}


//yield / resume

class Connector {
private:
    task *t;

public:
    // 存在几种策略：
    // 直接读取所有数据，将多余的数据保存在缓存中，直到无数据可读 yield，
    // 只读取所需要的数据。
    void read_until(void *data, int size) {
     


        // read
        printf("read some bytes\n");


        // 下面两行必须加锁，保证一起执行
        task_wait_list.push_back(t);
        swapcontext(&t->context, &t->parent);

        printf("read remain bytes\n");

        // read
    }
};

// 任务在accept时创建，一创建就会立刻尝试读取数据，直到无数据可读然后挂起



void server(Connector &conn) {

    conn.accept();


    char data[100];
    conn.read_until(data, 4);
}

void thread() {
    while (1) {
        if (!task_queue.empty()) {
            task *t = task_queue.front();
            task_queue.pop();

            swapcontext(&t->parent, &t->context);
        } else {
            sleep(1);
        }
    }
}

int main(void) {

    return 0;
}