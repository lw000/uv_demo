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
    struct timeval timeout = { 2, 500000 }; // 2.5 seconds
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

int RedisServer::setValue(const std::string& path, const std::string& key, const std::string& value) {
    if (key.empty()) {
        return -1;
    }
    
    if (value.empty()) {
        return -1;
    }
    
    redisReply *reply = (redisReply *)redisCommand(c, "SET %s %s", std::string(path + key).c_str(), value.c_str());
    int r = -1;
    if (reply && strcmp(reply->str, "OK") == 0) {
        r = 0;
    }
    freeReplyObject(reply);
    return r;
}

std::string RedisServer::getValue(const std::string& path, const std::string& key) {
    if (key.empty()) {
        return "";
    }
    
    redisReply *reply = (redisReply *)redisCommand(c, "GET %s", std::string(path + key).c_str());
    std::string result;
    if (reply) {
        if (reply->str != NULL) {
            result.append(reply->str);
        }
    }
    freeReplyObject(reply);
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

RedisAsyncServer::RedisAsyncServer() {
    this->loop = nullptr;
    this->ac = nullptr;
}

RedisAsyncServer::~RedisAsyncServer() {
    if (ac != NULL) {
        redisAsyncDisconnect(ac);
    }
    
    if (ac != NULL) {
        redisAsyncFree(ac);
    }
    
    if (loop != NULL) {
        free(loop);
    }
}

int RedisAsyncServer::start(const char *ip, int port) {
    loop = uv_loop_new();
    loop->data = this;
    
    ac = redisAsyncConnect(ip, port);
    ac->data = this;
    if (ac->err) {
        printf("error: %s\n", ac->errstr);
        return -1;
    }
    
    redisLibuvAttach(ac, loop);
    redisAsyncSetConnectCallback(ac, connectCallback);
    redisAsyncSetDisconnectCallback(ac, disconnectCallback);
    
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
    
    redisAsyncCommand(ac, NULL, NULL, "SET %s %s", key.c_str(), value.c_str());

    return 0;
}

int RedisAsyncServer::getValue(const std::string& key, std::function<void(const std::string value)> func) {
    if (key.empty()) {
        return -1;
    }
    
    if (func == nullptr) {
        return -1;
    }
    
    redisAsyncCommand(ac, getCallback, (char*)key.c_str(), "GET %s", key.c_str());
    
    return 0;
}

void RedisAsyncServer::onConnect(int status) {
    
}

void RedisAsyncServer::onDisconnect(int status) {
    
}

