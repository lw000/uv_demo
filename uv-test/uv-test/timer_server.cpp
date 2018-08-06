//
//  timer_server.cpp
//  uv-test
//
//  Created by 李伟 on 2018/8/3.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include "timer_server.hpp"
#include "uv.h"

static uv_loop_t* loop;
static int counter = 0;

static void timer_cb(uv_timer_t* handle) {
    printf("timer called [%d]\n", ++counter);
}

int timer_run(int argc, char ** args) {
    loop = uv_loop_new();
    
    uv_timer_t timer;
    uv_timer_init(loop, &timer);
    uv_timer_start(&timer, timer_cb, 10000, 1);
    
    int c = uv_run(loop, UV_RUN_DEFAULT);
    
    return  0;
}
