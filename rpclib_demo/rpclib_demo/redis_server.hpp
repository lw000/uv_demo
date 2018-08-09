//
//  redis_server.hpp
//  kafka_demo
//
//  Created by 李伟 on 2018/8/8.
//  Copyright © 2018年 李伟. All rights reserved.
//

#ifndef redis_server_hpp
#define redis_server_hpp

#include <stdio.h>
#include <string>
#include <functional>
#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libuv.h>

class BaseCommand {
public:
    BaseCommand(redisContext *c);
    ~BaseCommand();
    
private:
    redisContext *c;
};

class HashCommand {
public:
    HashCommand(redisContext *c);
    ~HashCommand();
    
private:
    redisContext *c;
};

class RedisServer {
public:
    RedisServer();
    ~RedisServer();
    
private:
    RedisServer(const RedisServer &&) = delete;
    RedisServer& operator =(const RedisServer&) = delete;

public:
    int start(const char *ip = "127.0.0.1", int port = 6379);

public:
    int setValue(const std::string& path, const std::string& key, const std::string& value);
    std::string getValue(const std::string& path, const std::string& key);
    long long incr(const std::string key);
    long long incrby(const std::string key, long long v);
    double incrfloat(const std::string key, double v);
    long long decr(const std::string key);
    long long decrby(const std::string key, long long v);
    
public:
    
    
private:
    redisContext *c;
};

class RedisAsyncServer {
public:
    RedisAsyncServer();
    ~RedisAsyncServer();
    
private:
    RedisAsyncServer(const RedisAsyncServer &&) = delete;
    RedisAsyncServer& operator =(const RedisAsyncServer&) = delete;
    
public:
    int start(const char *ip = "127.0.0.1", int port = 6379);
    
public:
    int setValue(const std::string& key, const std::string& value);
    int getValue(const std::string& key, std::function<void(const std::string value)> func);
    
public:
    void onConnect(int status);
    void onDisconnect(int status);
    
private:
    uv_loop_t* loop;
    redisAsyncContext *ac;
};
    
#endif /* redis_server_hpp */
