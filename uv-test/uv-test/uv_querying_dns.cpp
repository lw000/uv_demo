//
//  uv_querying_dns.cpp
//  uv-test
//
//  Created by 李伟 on 2018/8/6.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include "uv_querying_dns.hpp"
#include <stdlib.h>

#include "uv.h"

static uv_loop_t* loop;

static void getaddrinfo_cb(uv_getaddrinfo_t* req,
                                 int status,
                                 struct addrinfo* res) {
    if (status == -1) {
        fprintf(stderr, "getaddrinfo call error %s\n", uv_strerror(status));
        return;
    }
    
    char addr[16 + 1] = {0};
    uv_ip4_name((struct sockaddr_in*)res->ai_addr, addr, 16);
    fprintf(stdout, "%s\n", addr);
    
//    uv_tcp_t *socket = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
//    uv_tcp_init(loop, socket);
//
//    uv_connect_t *connect_req = (uv_connect_t*) malloc(sizeof(uv_connect_t));
//    connect_req->data = (void*) socket;
//    uv_tcp_connect(connect_req, socket, *(struct sockaddr_in*) res->ai_addr, on_connect);
//
//    uv_freeaddrinfo(res);
}

int querting_dns_run(int argc, char** args) {

    loop = uv_loop_new();
    
    struct addrinfo hints;
    hints.ai_family = PF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = 0;
    
    uv_getaddrinfo_t resolver;
    int r = uv_getaddrinfo(loop, &resolver, getaddrinfo_cb, "service.klgwl.com", "80", &hints);
    if (r != 0) {
        fprintf(stderr, "getaddroinfo call error %s\n", uv_strerror(r));
        return -1;
    }
    
    return uv_run(loop, UV_RUN_DEFAULT);
}
