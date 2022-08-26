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

#include "arpc_server.h"
#include "arpc_task.h"

char httpResponse_hello[] = "HTTP/1.1 200 Ok\r\n"
                            "Context-type: text/plain\r\n"
                            "Context-length: 20\r\n"
                            "\r\n"
                            "Hi! I am a messsage!";

#include "arpc_message.pb.h"

int main() {
    ell_Ipv4Addr local_addr;

    Server server(local_addr);

    server.message = [](Connector *conn) {
        static arpc::arpc_message message;
        static char data[128];

        memset(data, '\0', 128);

        conn->read_nbytes(data, 4);
        int32_t len = *(int32_t *)(data);

        printf("read %d bytes\n", len);

        conn->read_nbytes(data+4, len);
        auto m = message.ParseFromArray(&data[4], len);

        printf("m: %s \n", message.content().data());
    };

    server.start();

    return 0;
}