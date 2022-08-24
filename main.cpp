#include "Server.h"
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <map>
#include <queue>
#include <stdio.h>
#include <string>
#include <sys/types.h>
#include <thread>
#include <ucontext.h>
#include <unistd.h>
#include <vector>

#include "Server.h"
#include "task.h"

void thread() {
    while (1) {
        if (!task_queue.empty()) {
            task *t = task_queue.front();
            task_queue.pop();

            // if (getcontext(&t->parent) == -1) {
            //     printf("make task failture! \n");
            // }

            printf("test: %d \n", 88);

            // printf("task: %d \n", 88);

            // swapcontext(&t->parent, &t->context);
            t->resume();

            printf("from parent \n");
        } else {
            sleep(1);
            printf("sleep \n");
        }
    }
}

void my_Message(Connector *conn) {
    char data[64];

    while (1) {
        memset(data, '\0', 64);
        conn->read_nbytes(data, 64);

        // conn->read_until(data, 4);

        printf("read %d bytes:\n%s \n", 64, data);

        conn->send_nbytes(data, 64);
        // conn->send_nbytes((void *)"hello", 5);
    }
}

int main() {
    // std::thread t(thread);
    // std::thread t(thread);
    // t.detach();

    ell_Ipv4Addr local_addr;

    Server server(local_addr);

    server.message = [](Connector *conn) {
        char data[64];

        while (1) {
            memset(data, '\0', 64);
            conn->read_nbytes(data, 64);

            // conn->read_until(data, 4);

            printf("read %d bytes:\n%s \n", 64, data);

            conn->send_nbytes(data, 64);
            // conn->send_nbytes((void *)"hello", 5);
        }
    };

    // server.setdefaultMessage(my_Message);

    // server.accept();
    server.start();

    // t.join();
    return 0;
}