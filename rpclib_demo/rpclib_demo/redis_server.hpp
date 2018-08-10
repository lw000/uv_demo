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
class StringCommand;
class HashCommand;
class ListCommand;
class SetCommand;
class RedisBaseServer;
class RedisServer;
class RedisAsyncServer;


class Command {
public:
    Command();
    virtual ~Command();
    
public:
    void setContext(RedisBaseServer * srv, redisContext *c, redisAsyncContext *ac = NULL);
    
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
    long long del(const std::string& key, const std::string& path = "");
    bool exists(const std::string& key, const std::string& path = "");
    
public:
    long long expire(const std::string& key, int seconds, const std::string& path = "");
    long long pexpire(const std::string& key, int milliseconds, const std::string& path = "");
    long long expireat(const std::string& key, int timestamp, const std::string& path = "");
    
public:
    long long ttl(const std::string& key, const std::string& path = "");
    long long pttl(const std::string& key, const std::string& path = "");
};

class StringCommand : public Command {
public:
    StringCommand();
    virtual ~StringCommand();
    
public:
    long long set(const std::string& key, const std::string& value, const std::string& path = "");
    std::string get(const std::string& key, const std::string& path = "");
    
    long long mset(const std::map<std::string, std::string>& mvks, const std::string& path = "");
    std::vector<std::string> mget(const std::vector<std::string>& vks, const std::string& path = "");
    
    long long setnx(const std::string& key, const std::string& value, const std::string& path = "");
    long long setex(const std::string& key, const std::string& value, int seconds, const std::string& path = "");
    long long psetex(const std::string& key, const std::string& value, int milliseconds, const std::string& path = "");
    
    long long msetnx(const std::map<std::string, std::string>& keyvalues, const std::string& path = "");
    
    std::string getset(const std::string& key, const std::string& value, const std::string& path = "");
    
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
    long long hmset(const std::string& key, const std::string& fieldvalues, const std::string& path = "");
    long long hmset(const std::string& key, const std::map<std::string, std::string>& fieldvalues, const std::string& path = "");
    bool hexists(const std::string& key, const std::string& field, const std::string& path = "");
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
    
public:
    long long sadd(const std::string& key, const std::vector<std::string>& values, const std::string& path = "");
    std::vector<std::string> smembers(const std::string& key, const std::string& path = "");
};

class ListCommand : public Command {
public:
    ListCommand();
    virtual ~ListCommand();
};


class RedisBaseServer {
    friend BaseCommand;
    friend StringCommand;
    friend ListCommand;
    friend HashCommand;
    friend SetCommand;
    
public:
    RedisBaseServer();
    virtual ~RedisBaseServer();
    
private:
    RedisBaseServer(const RedisBaseServer &&) = delete;
    RedisBaseServer& operator =(const RedisBaseServer&) = delete;
    
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

public:
    int start(const char *ip = "127.0.0.1", int port = 6379, int db = 0);
    bool auth(const std::string& psd);
    long long select(int db);
    
    long long setConfig(const std::string& key, const std::string& value);
    std::map<std::string, std::string> getConfig(const std::string& key = "*");
    
public:
    long long ping();
    
public:
    BaseCommand* baseCommand();
    StringCommand* stringCommand();
    HashCommand* hashCommand();
    SetCommand* setCommand();
    
private:
    BaseCommand* baseCmd;
    StringCommand* stringCmd;
    HashCommand* hashCmd;
    SetCommand* setCmd;
    
private:
    redisContext *c;
};

class RedisServerPool {
public:
    RedisServerPool();
    ~RedisServerPool();

public:
    
};

class RedisAsyncServer {
public:
    RedisAsyncServer();
    virtual ~RedisAsyncServer();
    
public:
    int start(const char *ip = "127.0.0.1", int port = 6379);
    
public:
    int set(const std::string& key, const std::string& value, std::function<void(const std::string value)> func);
    int get(const std::string& key, std::function<void(const std::string value)> func);
    
public:
    void onConnect(int status);
    void onDisconnect(int status);
    
private:
    uv_loop_t* loop;
    redisAsyncContext *ac;
};
    
#endif /* redis_server_hpp */
