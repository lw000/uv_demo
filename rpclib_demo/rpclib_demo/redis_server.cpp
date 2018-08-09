//
//  redis_server.cpp
//  kafka_demo
//
//  Created by 李伟 on 2018/8/8.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include "redis_server.hpp"

#include <pthread.h>

static void getCallback(redisAsyncContext * c, void *r, void *privdata) {
    RedisAsyncServer* srv = reinterpret_cast<RedisAsyncServer*>(c->data);
    
    redisReply* reply = (redisReply*)r;
    if (reply == NULL) {
        return;
    }
    printf("argv[%s]:%s\n", (char*)privdata, reply->str);
    

    
    redisAsyncDisconnect(c);
}

static void connectCallback(const redisAsyncContext* c, int status) {
    RedisAsyncServer* srv = reinterpret_cast<RedisAsyncServer*>(c->data);
    srv->onConnect(status);
    
    if (status != REDIS_OK) {
        printf("error:%s\n", c->errstr);
        return;
    }
    printf("connected...\n");
    
}

static void disconnectCallback(const redisAsyncContext* c, int status) {
    RedisAsyncServer* srv = reinterpret_cast<RedisAsyncServer*>(c->data);
    srv->onDisconnect(status);
    
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
    
    uv_loop_close(loop);
    
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
    this->c = nullptr;
}

RedisServer::~RedisServer() {
    if (c != NULL) {
        redisFree(c);
    }
}

int RedisServer::start(const char *ip, int port) {
    
    c = redisConnect(ip, port);
    if (c->err) {
        printf("error: %s\n", c->errstr);
        return -1;
    }
    struct timeval timeout = { 5, 500000 }; // 1.5 seconds
    c = redisConnectWithTimeout(ip, port, timeout);
    if (c == NULL || c->err) {
        if (c) {
            printf("connection error: %s\n", c->errstr);
            redisFree(c);
        } else {
            printf("connection error: can't allocate redis context\n");
        }
        return -1;
    }
    
    return 0;
}

int RedisServer::setValue(const std::string& key, const std::string& value) {
    if (key.empty()) {
        return -1;
    }
    
    if (value.empty()) {
        return -1;
    }
    
    redisReply *reply = (redisReply *)redisCommand(c, "SET %s %s", key.c_str(), value.c_str());
    int c = -1;
    if (reply && strcmp(reply->str, "ok") == 0) {
        c = 0;
    }
    freeReplyObject(reply);
    return c;
}

std::string RedisServer::getValue(const std::string& key) {
    if (key.empty()) {
        return "";
    }
    
    redisReply *reply = (redisReply *)redisCommand(c, "GET %s", key.c_str());
    std::string result;
    if (reply && strcmp(reply->str, "ok") == 0) {
        result.append(reply->str);
    }
    freeReplyObject(reply);
    return "";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

RedisAsyncServer::RedisAsyncServer() {
    this->loop = nullptr;
    this->c = nullptr;
}

RedisAsyncServer::~RedisAsyncServer() {
    if (c != NULL) {
        redisAsyncDisconnect(c);
    }
    
    if (c != NULL) {
        redisAsyncFree(c);
    }
    
    if (loop != NULL) {
        free(loop);
    }
}

int RedisAsyncServer::start(const char *ip, int port) {
    loop = uv_loop_new();
    loop->data = this;
    
    c = redisAsyncConnect(ip, port);
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

int RedisAsyncServer::setValue(const std::string& key, const std::string& value) {
    if (key.empty()) {
        return -1;
    }
    
    if (value.empty()) {
        return -1;
    }
    
    redisAsyncCommand(c, NULL, NULL, "SET %s %s", key.c_str(), value.c_str());

    return 0;
}

int RedisAsyncServer::getValue(const std::string& key, std::function<void(const std::string value)> func) {
    if (key.empty()) {
        return -1;
    }
    
    if (func == nullptr) {
        return -1;
    }
    
    redisAsyncCommand(c, getCallback, (char*)key.c_str(), "GET %s", key.c_str());
    
    return 0;
}

void RedisAsyncServer::onConnect(int status) {
    
}

void RedisAsyncServer::onDisconnect(int status) {
    
}

