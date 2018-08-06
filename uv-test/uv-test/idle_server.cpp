//
//  idle_server.cpp
//  uv-test
//
//  Created by 李伟 on 2018/8/3.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include "idle_server.hpp"

#include "uv.h"

static uv_loop_t* loop;

static void idle_cb(uv_idle_t* handle) {
    
}

int idle_run(int argc, char** args) {
    loop = uv_loop_new();
    
    uv_idle_t idle;
    uv_idle_init(loop, &idle);
    uv_idle_start(&idle, idle_cb);
    int c = uv_run(loop, UV_RUN_DEFAULT);
    if (c == 0) {
        
    }
    
    return 0;
}
