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
#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libuv.h>

#include <functional>

int start_redis_main_server(int argc, const char * argv[]);

class RedisServer {
public:
    RedisServer();
    ~RedisServer();
    
public:
    int start(const char *ip = "127.0.0.1", int port = 6379);

public:
    int setValue(const std::string& key, const std::string& value);
    std::string getValue(const std::string& key);

private:
    redisContext *c;
};

class RedisAsyncServer {
public:
    RedisAsyncServer();
    ~RedisAsyncServer();
    
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
    redisAsyncContext *c;
};
    
#endif /* redis_server_hpp */
