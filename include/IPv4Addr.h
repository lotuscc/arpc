#pragma once

#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>

class ell_Ipv4Addr {
private:
    struct sockaddr_in addr_;
    socklen_t len_;
public:
    void remake(const char *ip = "127.0.0.1", int port = 9999);
    void remake();

public:
    ell_Ipv4Addr(const char *ip = "127.0.0.1", int port = 9999);
    ~ell_Ipv4Addr();

    // ell_Ipv4Addr(const ell_Ipv4Addr &) = delete;
    // ell_Ipv4Addr &operator=(const ell_Ipv4Addr &) = delete;

    struct sockaddr *addr(void);
    socklen_t len(void);

    socklen_t *len_addr();
};