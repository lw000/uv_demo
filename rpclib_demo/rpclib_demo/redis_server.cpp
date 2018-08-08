//
//  redis_server.cpp
//  kafka_demo
//
//  Created by 李伟 on 2018/8/8.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include "redis_server.hpp"
#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libuv.h>

#include <pthread.h>

static void getCallback(redisAsyncContext * c, void *r, void *privdata) {
    RedisServer* srv = reinterpret_cast<RedisServer*>(c->data);
    
    redisReply* reply = (redisReply*)r;
    if (reply == NULL) {
        return;
    }
    printf("argv[%s]:%s\n", (char*)privdata, reply->str);
    
    srv->onConnect(status);
    
    redisAsyncDisconnect(c);
}

static void connectCallback(const redisAsyncContext* c, int status) {
    RedisServer* srv = reinterpret_cast<RedisServer*>(c->data);
    
    if (status != REDIS_OK) {
        printf("error:%s\n", c->errstr);
        return;
    }
    printf("connected...\n");
    
    srv->onDisconnect(status);
}

static void disconnectCallback(const redisAsyncContext* c, int status) {
    if (status != REDIS_OK) {
        printf("error:%s\n", c->errstr);
        return;
    }
    printf("disconnected...\n");
}

static void* enter(void* args) {
    uv_loop_t* loop = (uv_loop_t*)args;
    
    int r = uv_run(loop, UV_RUN_DEFAULT);
    if (r != 0) {
        
    }
    
    return 0;
}

int start_redis_main_server(int argc, const char * argv[]) {
//    ::signal(SIGPIPE, SIG_IGN);

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
    
    pthread_t pid;
    pthread_create(&pid, NULL, enter, loop);
    
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

RedisServer::RedisServer() {

}

RedisServer::~RedisServer() {
    
}

int RedisServer::start() {
    uv_loop_t* loop = uv_loop_new();
    loop->data = this;
    
    redisAsyncContext *c = redisAsyncConnect("127.0.0.1", 6379);
    c->data = this;
    if (c->err) {
        printf("error: %s\n", c->errstr);
        return -1;
    }
    
    redisLibuvAttach(c, loop);
    redisAsyncSetConnectCallback(c, connectCallback);
    redisAsyncSetDisconnectCallback(c, disconnectCallback);
    
    pthread_t pid;
    pthread_create(&pid, NULL, enter, loop);
    
    return 0;
}

void RedisServer::onConnect(int status) {
    
}

void RedisServer::onDisconnect(int status) {
    
}

