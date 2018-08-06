//
//  timer_server.cpp
//  uv-test
//
//  Created by 李伟 on 2018/8/3.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include "timer_server.hpp"
#include <stdlib.h>
#include <vector>

#include "uv.h"

static uv_loop_t* loop;
//static int idle_counter = 0;

static void timer_cb(uv_timer_t* handle) {
    printf("timer id = [%d], time = %d called.\n", *((int*)handle->data), handle->repeat/1000);
}

static void idle_cb(uv_idle_t* handle) {
//    printf("idle called [%d]\n", ++idle_counter);
}

struct SHARED_DATA {
    uv_timer_t* timer;
};

std::vector<uv_timer_t*> tms;

int timer_run(int argc, char ** args) {
    
    srand(time(NULL));
    
    loop = uv_loop_new();
    
//    uv_idle_t idle;
//    uv_idle_init(loop, &idle);
//    uv_idle_start(&idle, idle_cb);
    
    for (int i = 0; i < 100000; i++) {
        uv_timer_t* timer = (uv_timer_t*) :: malloc(sizeof(uv_timer_t));
        timer->data = new int(i);
        uv_timer_init(loop, timer);
        uv_timer_start(timer, timer_cb, 0, 1000 + rand() % (3600*1000));
//        uv_timer_start(timer, timer_cb, 0, 0);
        tms.push_back(timer);
    }
    
//    uv_timer_t timer;
//    uv_timer_init(loop, &timer);
//    uv_timer_start(&timer, timer_cb, 0, 1000);
    
    int c = uv_run(loop, UV_RUN_DEFAULT);
    if (c == 0) {
        
    }
    
    for (auto v : tms) {
        delete (int*)v->data;
        free(v);
    }
    
    return  0;
}
