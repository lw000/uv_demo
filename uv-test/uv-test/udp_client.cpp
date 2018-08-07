//
//  udp_client.cpp
//  uv-test
//
//  Created by 李伟 on 2018/8/7.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include "udp_client.hpp"
#include <stdlib.h>

#include "uv.h"

static uv_loop_t * loop;

static void alloc_cb(uv_handle_t* handle,
                     size_t suggested_size,
                     uv_buf_t* buf) {
    
}

static void recv_cb(uv_udp_t* handle,
                    ssize_t nread,
                    const uv_buf_t* buf,
                    const struct sockaddr* addr,
                    unsigned flags) {
    
}

static void send_cb(uv_udp_send_t* req, int status) {
    if (status) {
        fprintf(stderr, "Send error %s\n", uv_strerror(status));
        return;
    }
}

int udp_client_run(int argc, char **args) {
    
    loop = uv_loop_new();
    
    uv_udp_t send_socket;
    uv_udp_init(loop, &send_socket);
    struct sockaddr_in addr;
    uv_ip4_addr("0.0.0.0", 9987, &addr);

    uv_buf_t buf;
    buf.base = (char*)::malloc(100);
    buf.len = 100;
    
    uv_udp_send_t send_req;
    uv_udp_send(&send_req, &send_socket, &buf, 1, (const struct sockaddr*)&addr, send_cb);
    
    int r = uv_run(loop, UV_RUN_DEFAULT);
    if (r == 0) {
        
    }
    free(loop);
    
    return 0;
}
