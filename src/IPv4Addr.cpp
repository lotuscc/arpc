#pragma once

#include "IPv4Addr.h"
#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>

// NetAddress


void ell_Ipv4Addr::remake(const char *ip, int port) {
    bzero(&addr_, sizeof(addr_));
    addr_.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &addr_.sin_addr);
    addr_.sin_port = htons(port);

    len_ = sizeof addr_;
}

ell_Ipv4Addr::ell_Ipv4Addr(const char *ip, int port) {
    bzero(&addr_, sizeof(addr_));
    addr_.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &addr_.sin_addr);
    addr_.sin_port = htons(port);

    len_ = sizeof addr_;
}

ell_Ipv4Addr::~ell_Ipv4Addr() {}

struct sockaddr *ell_Ipv4Addr::addr(void) {
    return (struct sockaddr *)&addr_;
}

socklen_t ell_Ipv4Addr::len(void) { return len_; }

socklen_t *ell_Ipv4Addr::len_addr() { return (socklen_t *)&len_; }