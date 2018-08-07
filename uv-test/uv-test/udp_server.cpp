//
//  udp_server.cpp
//  uv-test
//
//  Created by 李伟 on 2018/8/7.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include "udp_server.hpp"
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

int udp_server_run(int argc, char **args) {

    loop = uv_loop_new();
    
    uv_udp_t cli;
    uv_udp_init(loop, &cli);
    struct sockaddr_in addr;
    uv_ip4_addr("0.0.0.0", 9987, &addr);
    uv_udp_bind(&cli, (const struct sockaddr*)&addr, UV_UDP_REUSEADDR);
    uv_udp_recv_start(&cli, alloc_cb, recv_cb);
    
    
    int r = uv_run(loop, UV_RUN_DEFAULT);
    if (r == 0) {
        
    }
    free(loop);
    
    return 0;
}
