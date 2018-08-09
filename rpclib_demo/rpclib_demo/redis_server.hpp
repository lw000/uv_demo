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
#include <vector>
#include <map>
#include <pthread.h>
#include <functional>
#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libuv.h>

class Command;
class BaseCommand;
class HashCommand;
class RedisBaseServer;
class RedisServer;
class RedisAsyncServer;

class Command {
public:
    Command();
    virtual ~Command();
    
public:
    void setContext(RedisBaseServer * srv, redisContext *c, redisAsyncContext *ac);
    
protected:
    redisContext *c;
    redisAsyncContext *ac;
    RedisBaseServer * srv;
};

class BaseCommand : public Command {
public:
    BaseCommand();
    virtual ~BaseCommand();
    
public:
    long long set(const std::string& key, const std::string& value, const std::string& path = "");
    std::string get(const std::string& key, const std::string& path = "");
    std::string setget(const std::string& key, const std::string& value, const std::string& path = "");
    long long incr(const std::string key);
    long long incrby(const std::string key, long long v);
    double incrfloat(const std::string key, double v);
    long long decr(const std::string key);
    long long decrby(const std::string key, long long v);
};

class HashCommand : public Command {
public:
    HashCommand();
    virtual ~HashCommand();
    
public:
    long long hset(const std::string& key, const std::string& field, const std::string& value, const std::string& path = "");
    long long hmset(const std::string& key, const std::map<std::string, std::string>& fieldvalues, const std::string& path = "");
    long long hexists(const std::string& key, const std::string& field, const std::string& path = "");
    long long hdel(const std::string& key, const std::string& field, const std::string& path = "");
    std::string hget(const std::string& key, const std::string& field, const std::string& path = "");
    std::map<std::string, std::string> hgetall(const std::string& key, const std::string& path = "");
    
    long long hlen(const std::string& key, const std::string& path = "");
    std::vector<std::string> hkeys(const std::string& key, const std::string& path = "");
};

class SetCommand : public Command {
public:
    SetCommand();
    virtual ~SetCommand();
};

class RedisBaseServer {
    friend BaseCommand;
    friend HashCommand;
public:
    RedisBaseServer();
    virtual ~RedisBaseServer();
    
protected:
    void lock();
    void unlock();
    
protected:
    pthread_mutex_t t;
};

class RedisServer : public RedisBaseServer {
public:
    RedisServer();
    virtual ~RedisServer();
    
private:
    RedisServer(const RedisServer &&) = delete;
    RedisServer& operator =(const RedisServer&) = delete;

public:
    int start(const char *ip = "127.0.0.1", int port = 6379);

public:
    BaseCommand* baseCommand();
    HashCommand* hashCommand();
    
private:
    BaseCommand baseCmd;
    HashCommand hashCmd;
    redisContext *c;
};

class RedisAsyncServer {
public:
    RedisAsyncServer();
    virtual ~RedisAsyncServer();
    
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
