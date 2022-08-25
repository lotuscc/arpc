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

char httpResponse_hello[] = \
"HTTP/1.1 200 Ok\r\n" \
"Context-type: text/plain\r\n" \
"Context-length: 20\r\n" \
"\r\n" \
"Hi! I am a messsage!";


int main() {
    ell_Ipv4Addr local_addr;

    Server server(local_addr);

    server.message = [](Connector *conn) {
        static char data[128];

        // while (1) {
            memset(data, '\0', 128);
            conn->read_nbytes(data, 78);

            printf("read %d bytes:\n%s \n", 78, data);

            conn->send_nbytes(httpResponse_hello, sizeof(httpResponse_hello)-1);   // -1 去掉结尾的'\0'                 
                        
        // }

        // 不需要手动close，该函数结束之后会自动close
        // 如果想提前close，请使用return，直接返回
        // conn->close();
    };

    // server.setdefaultMessage(my_Message);

    server.start();

    return 0;
}