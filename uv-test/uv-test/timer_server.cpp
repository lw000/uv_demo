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
static int timer_counter = 0;
static int idle_counter = 0;

static void timer_cb(uv_timer_t* handle) {
    printf("timer called [%d]\n", ++timer_counter);
}

static void idle_cb(uv_idle_t* handle) {
//    printf("idle called [%d]\n", ++idle_counter);
}

int timer_run(int argc, char ** args) {
    loop = uv_loop_new();
    
    uv_idle_t idle;
    uv_idle_init(loop, &idle);
    uv_idle_start(&idle, idle_cb);
    
    uv_timer_t timer;
    uv_timer_init(loop, &timer);
    uv_timer_start(&timer, timer_cb, 0, 1000);
    
    int c = uv_run(loop, UV_RUN_DEFAULT);
    if (c == 0) {
        
    }
    
    return  0;
}
