//
//  main.cpp
//  redis-demo
//
//  Created by 李伟 on 2018/8/8.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include <iostream>
#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libuv.h>

static void getCallback(redisAsyncContext * c, void *r, void *privdata) {
    redisReply* reply = (redisReply*)r;
    if (reply == NULL) {
        return;
    }
    printf("argv[%s]:%s\n", (char*)privdata, reply->str);
    
    redisAsyncDisconnect(c);
}

static void connectCallback(const redisAsyncContext* c, int status) {
    if (status != REDIS_OK) {
        printf("error:%s\n", c->errstr);
        return;
    }
    printf("connected...\n");
}

static void disconnectCallback(const redisAsyncContext* c, int status) {
    if (status != REDIS_OK) {
        printf("error:%s\n", c->errstr);
        return;
    }
    printf("disconnected...\n");
}

int main(int argc, const char * argv[]) {
    ::signal(SIGPIPE, SIG_IGN);
    
    uv_loop_t* loop = uv_loop_new();
    
    redisAsyncContext *c = redisAsyncConnect("127.0.0.1", 6379);
    if (c->err) {
        printf("error: %s\n", c->errstr);
        return 0;
    }
    
    redisLibuvAttach(c, loop);
    redisAsyncSetConnectCallback(c, connectCallback);
    redisAsyncSetDisconnectCallback(c, disconnectCallback);
    
    redisAsyncCommand(c, NULL, NULL, "SET %s %s", "name", "liwei");
    redisAsyncCommand(c, getCallback, (char*)"end-1", "GET %s", "name");
    
    return uv_run(loop, UV_RUN_DEFAULT);;
}
