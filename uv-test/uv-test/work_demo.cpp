//
//  work_demo.cpp
//  uv-test
//
//  Created by 李伟 on 2018/8/7.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include "work_demo.hpp"
#include <stdlib.h>
#include <unistd.h>

#include "uv.h"

static uv_loop_t* loop;
static uv_async_t async;

static void async_cb(uv_async_t* handle) {
    double percentage = (*(double*)handle->data);
    fprintf(stderr, "download %.2f\n", percentage);
}

static void work_cb(uv_work_t* req) {
    int size = *((int*)req->data);
    int downloaded = 0;
    double percentage;
    while (downloaded < size) {
        percentage = downloaded * 100.0 / size;
        async.data = &percentage;
        uv_async_send(&async);
        sleep(1);
        downloaded += (200 + random()) % 1000;
    }
}

static void after_work_cb(uv_work_t* req, int status) {
    fprintf(stderr, "download complete\n");
    uv_close((uv_handle_t*)&async, NULL);
}

int work_demo_run(int argc, char** args) {
    loop = uv_loop_new();
    
    uv_work_t req;
    int size = 10240;
    req.data = &size;
    
    uv_async_init(loop, &async, async_cb);
    uv_queue_work(loop, &req, work_cb, after_work_cb);
    
    int r = uv_run(loop, UV_RUN_DEFAULT);
    if (r == 0) {
        
    }
    
    return r;
}
