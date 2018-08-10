//
//  redis_server.cpp
//  kafka_demo
//
//  Created by 李伟 on 2018/8/8.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include "redis_server.hpp"
#include <sstream>
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

Command::Command() : c(NULL), ac(NULL) {

}

Command::~Command() {
    
}

void Command::setContext(RedisBaseServer * srv, redisContext *c, redisAsyncContext *ac) {
    this->c = c;
    this->ac = ac;
    this->srv = srv;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
BaseCommand::BaseCommand() {
    
}

BaseCommand::~BaseCommand() {
    
}


long long BaseCommand::pexpire(const std::string& key, int milliseconds, const std::string& path) {
    if (key.empty()) {
        return -1;
    }
    
    if (milliseconds < 0) {
        return -1;
    }
    
    redisReply *reply = NULL;
    {
        this->srv->lock();
        reply = (redisReply *)redisCommand(c, "PEXPIRE %s %d", std::string(path + key).c_str(), milliseconds);
        this->srv->unlock();
    }
    
    long long r = -1;
    if (reply) {
        if (reply->str == NULL) {
            r = reply->integer;
        } else {
            printf("%s\n", reply->str);
        }
    }
    freeReplyObject(reply);
    
    return r;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
StringCommand::StringCommand() {
    
}

StringCommand::~StringCommand() {
    
}

bool StringCommand::exists(const std::string& key, const std::string& path) {
    if (key.empty()) {
        return false;
    }
    
    redisReply *reply = NULL;
    {
        this->srv->lock();
        reply = (redisReply *)redisCommand(c, "EXISTS %s", std::string(path + key).c_str());
        this->srv->unlock();
    }
    
    bool r = false;
    if (reply) {
        if (reply->str == NULL) {
            r = (reply->integer == 1);
        } else {
            printf("%s\n", reply->str);
        }
    }
    freeReplyObject(reply);
    
    return r;
}

long long StringCommand::set(const std::string& key, const std::string& value, const std::string& path) {
    if (key.empty()) {
        return -1;
    }
    
    if (value.empty()) {
        return -1;
    }

    redisReply *reply = NULL;
    {
        this->srv->lock();
        reply = (redisReply *)redisCommand(c, "SET %s %s", std::string(path + key).c_str(), value.c_str());
        this->srv->unlock();
    }
    
    long long r = -1;
    if (reply && strcmp(reply->str, "OK") == 0) {
        r = reply->integer;
    }
    freeReplyObject(reply);
    
    return r;
}

std::string StringCommand::get(const std::string& key, const std::string& path) {
    if (key.empty()) {
        return "";
    }
    
    redisReply *reply = NULL;
    {
        this->srv->lock();
        reply = (redisReply *)redisCommand(c, "GET %s", std::string(path + key).c_str());
        this->srv->unlock();
    }

    std::string result;
    if (reply) {
        if (reply->str != NULL) {
            result.append(reply->str);
        }
    }
    freeReplyObject(reply);
    return result;
}

std::vector<std::string> StringCommand::mget(const std::vector<std::string>& vkeys, const std::string& path) {
    if (vkeys.empty()) {
        return std::vector<std::string>();
    }
    
    std::string str;
    for (auto m : vkeys) {
        str.append(m);
        str.append(" ");
    }
    
    redisReply *reply = NULL;
    {
        this->srv->lock();
        reply = (redisReply *)redisCommand(c, "MGET %s", std::string(path + str).c_str());
        this->srv->unlock();
    }
    
    std::vector<std::string> result;
    if (reply) {
        if (reply->str == NULL) {
            for (int i = 0; i < reply->elements; i++) {
                result.push_back(reply->element[i]->str);
            }
        }
    }
    freeReplyObject(reply);
    return result;
}

std::string StringCommand::getset(const std::string& key, const std::string& value, const std::string& path) {
    if (key.empty()) {
        return "";
    }
    
    redisReply *reply = NULL;
    {
        this->srv->lock();
        reply = (redisReply *)redisCommand(c, "GETSET %s, %s", std::string(path + key).c_str(), value.c_str());
        this->srv->unlock();
    }
    
    std::string result;
    if (reply) {
        if (reply->str != NULL) {
            result.append(reply->str);
        }
        else {
            printf("%s\n", reply->str);
        }
    }
    freeReplyObject(reply);
    return result;
}

long long StringCommand::mset(const std::map<std::string, std::string>& keyvalues, const std::string& path) {
    if (keyvalues.empty()) {
        return -1;
    }
    
//    if (fieldvalues.size() % 2 != 0) {
//        printf("fieldvalues error!\n");
//        return -1;
//    }
    
    std::ostringstream out;
    for (auto m : keyvalues) {
        out << std::string(path + m.first) << " " << m.second << " ";
    }
    std::string str(out.str());
    redisReply *reply = NULL;
    {
        this->srv->lock();
        reply = (redisReply *)redisCommand(c, "MSET %s", str.c_str());
        this->srv->unlock();
    }
    
    long long r = 0;
    if (reply) {
        if (strcmp(reply->str, "OK") == 0) {
            r = 1;
        }
        else {
            printf("%s\n", reply->str);
        }
    }
    freeReplyObject(reply);
    return r;
}

long long StringCommand::incr(const std::string key) {
    if (key.empty()) {
        return -1;
    }
    
    redisReply *reply = NULL;
    {
        this->srv->lock();
        reply = (redisReply *)redisCommand(c, "INCR %s", key.c_str());
        this->srv->unlock();
    }
    
    long long r = 0;
    if (reply) {
        if (reply->str == NULL) {
            r = reply->integer;
        }
        else {
            printf("%s\n", reply->str);
        }
    }
    freeReplyObject(reply);
    return r;
}

long long StringCommand::incrby(const std::string key, long long v) {
    if (key.empty()) {
        return -1;
    }
    redisReply *reply = NULL;
    {
        this->srv->lock();
        reply = (redisReply *)redisCommand(c, "INCRBY %s %lld", key.c_str(), v);
        this->srv->unlock();
    }
    long long r = 0;
    if (reply) {
        if (reply->str == NULL) {
            r = reply->integer;
        }
        else {
            printf("%s\n", reply->str);
        }
    }
    freeReplyObject(reply);
    return r;
}

double StringCommand::incrfloat(const std::string key, double v) {
    if (key.empty()) {
        return -1;
    }
    redisReply *reply = NULL;
    {
        this->srv->lock();
        reply = (redisReply *)redisCommand(c, "INCRBYFLOAT %s %f", key.c_str(), v);
        this->srv->unlock();
    }

    double r = 0.0;
    if (reply && reply->str != NULL) {
        r = atof(reply->str);
    }
    freeReplyObject(reply);
    return r;
}

long long StringCommand::decr(const std::string key) {
    if (key.empty()) {
        return -1;
    }
    redisReply *reply = NULL;
    {
        this->srv->lock();
        reply = (redisReply *)redisCommand(c, "DECR %s", key.c_str());
        this->srv->unlock();
    }

    long long r = 0;
    if (reply) {
        if (reply->str == NULL) {
            r = reply->integer;
        }
        else {
            printf("%s\n", reply->str);
        }
    }
    freeReplyObject(reply);
    return r;
}

long long StringCommand::decrby(const std::string key, long long v) {
    if (key.empty()) {
        return -1;
    }
    redisReply *reply = NULL;
    {
        this->srv->lock();
        reply = (redisReply *)redisCommand(c, "DECRBY %s %lld", key.c_str(), v);
        this->srv->unlock();
    }

    long long r = 0;
    if (reply) {
        if (reply->str == NULL) {
            r = reply->integer;
        }
        else {
            printf("%s\n", reply->str);
        }
    }
    freeReplyObject(reply);
    return r;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
HashCommand::HashCommand() {
    
}

HashCommand::~HashCommand() {
    
}

long long HashCommand::hset(const std::string& key, const std::string& field, const std::string& value, const std::string& path) {
    if (key.empty()) {
        return -1;
    }
    
    if (field.empty()) {
        return -1;
    }
   
    redisReply *reply = NULL;
    {
        this->srv->lock();
        reply = (redisReply *)redisCommand(c, "HSET %s %s %s", std::string(path + key).c_str(), field.c_str(), value.c_str());
        this->srv->unlock();
    }

    long long r = -1;
    if (reply) {
        if (reply->str == NULL) {
            r = reply->integer;
        }
        else {
            printf("%s\n", reply->str);
        }
    }
    freeReplyObject(reply);
    return r;
}

long long HashCommand::hmset(const std::string& key, const std::map<std::string, std::string>& fieldvalues, const std::string& path) {
    if (key.empty()) {
        return -1;
    }
    
    if (fieldvalues.empty()) {
        return -1;
    }
    
//    if (fieldvalues.size() % 2 != 0) {
//        printf("fieldvalues error!\n");
//        return -1;
//    }
    
    std::ostringstream out;
    for (auto m : fieldvalues) {
        out << m.first << " " << m.second << " ";
    }
    std::string cmds(out.str());
    redisReply *reply = NULL;
    {
        this->srv->lock();
        reply = (redisReply *)redisCommand(c, "HMSET %s %s", std::string(path + key).c_str(), cmds.c_str());
        this->srv->unlock();
    }

    long long r = -1;
    if (reply) {
        if (reply->str == NULL) {
            r = reply->integer;
        }
        else {
            printf("%s\n", reply->str);
        }
    }
    freeReplyObject(reply);
    return r;
}

long long HashCommand::hmset(const std::string& key, const std::string& fieldvalues, const std::string& path) {
    if (key.empty()) {
        return -1;
    }
    
    if (fieldvalues.empty()) {
        return -1;
    }
    
    redisReply *reply = NULL;
    {
        this->srv->lock();
        reply = (redisReply *)redisCommand(c, "HMSET %s %s ", std::string(path + key).c_str(), fieldvalues.c_str());
        this->srv->unlock();
    }
    
    long long r = -1;
    if (reply) {
        if (strcmp(reply->str, "OK") == 0) {
            r = reply->integer;
        }
        else {
            printf("%s\n", reply->str);
        }
    }
    freeReplyObject(reply);
    return r;
}

bool HashCommand::hexists(const std::string& key, const std::string& field, const std::string& path) {
    if (key.empty()) {
        return false;
    }
    
    if (field.empty()) {
        return false;
    }
    
    redisReply *reply = NULL;
    {
        this->srv->lock();
        reply = (redisReply *)redisCommand(c, "HEXISTS %s %s", std::string(path + key).c_str(), field.c_str());
        this->srv->unlock();
    }

    bool r = false;
    if (reply) {
        if (reply->str == NULL) {
            r = (reply->integer == 1);
        }
        else {
            printf("%s\n", reply->str);
        }
    }
    freeReplyObject(reply);
    return r;
}

long long HashCommand::hdel(const std::string& key, const std::string& field, const std::string& path) {
    if (key.empty()) {
        return -1;
    }
    
    if (field.empty()) {
        return -1;
    }
    redisReply *reply = NULL;
    {
        this->srv->lock();
        reply = (redisReply *)redisCommand(c, "HDEL %s %s", std::string(path + key).c_str(), field.c_str());
        this->srv->unlock();
    }

    long long r = -1;
    if (reply) {
        if (reply->str == NULL) {
            r = reply->integer;
        }
        else {
            printf("%s\n", reply->str);
        }
    }
    freeReplyObject(reply);
    return r;
}

std::string HashCommand::hget(const std::string& key, const std::string& field, const std::string& path) {
    if (key.empty()) {
        return "";
    }
    
    if (field.empty()) {
        return "";
    }
    redisReply *reply = NULL;
    {
        this->srv->lock();
        reply = (redisReply *)redisCommand(c, "HGET %s %s", std::string(path + key).c_str(), field.c_str());
        this->srv->unlock();
    }

    std::string result;
    if (reply) {
        if (reply->str != NULL) {
            result.append(reply->str);
        }
        else {
            printf("%s\n", reply->str);
        }
    }
    freeReplyObject(reply);
    return result;
}

std::map<std::string, std::string> HashCommand::hgetall(const std::string& key, const std::string& path) {
    if (key.empty()) {
        return std::map<std::string, std::string>();
    }
    redisReply *reply = NULL;
    {
        this->srv->lock();
        reply = (redisReply *)redisCommand(c, "HGETALL %s", std::string(path + key).c_str());
        this->srv->unlock();
    }

    std::map<std::string, std::string> elems;
    
    if (reply) {
        if (reply->str == NULL) {
            for (int i = 0; i < reply->elements; i=i+2) {
                elems.insert(std::make_pair(reply->element[i]->str, reply->element[i+1]->str));
            }
        }
        else {
            printf("%s\n", reply->str);
        }
    }
    freeReplyObject(reply);
    return elems;
}

long long HashCommand::hlen(const std::string& key, const std::string& path) {
    if (key.empty()) {
        return -1;
    }
    
    long long result = 0;
    redisReply *reply = NULL;
    {
        this->srv->lock();
        reply = (redisReply *)redisCommand(c, "HLEN %s", std::string(path + key).c_str());
        this->srv->unlock();
    }

    if (reply) {
        if (reply->str == NULL) {
            result = reply->integer;
        }
        else {
            printf("%s\n", reply->str);
        }
    }
    freeReplyObject(reply);
    return result;
}

std::vector<std::string> HashCommand::hkeys(const std::string& key, const std::string& path) {
    if (key.empty()) {
        return std::vector<std::string>();
    }
    redisReply *reply = NULL;
    {
        this->srv->lock();
        reply = (redisReply *)redisCommand(c, "HKEYS %s", std::string(path + key).c_str());
        this->srv->unlock();
    }

    std::vector<std::string> elems;
    
    if (reply) {
        if (reply->str == NULL) {
            for (int i = 0; i < reply->elements; i++) {
                elems.push_back(reply->element[i]->str);
            }
        }
        else {
            printf("%s\n", reply->str);
        }
    }
    freeReplyObject(reply);
    return elems;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
SetCommand::SetCommand() {
    
}

SetCommand::~SetCommand() {
    
}

////////////////////////////////////////////////////////////////////////////////////////////////////
ListCommand::ListCommand() {
    
}

ListCommand::~ListCommand() {
    
}

////////////////////////////////////////////////////////////////////////////////////////////////////
RedisBaseServer::RedisBaseServer() {
    pthread_mutex_init(&t, NULL);
}

RedisBaseServer::~RedisBaseServer() {
    pthread_mutex_destroy(&t);
}

void RedisBaseServer::lock() {
    pthread_mutex_lock(&t);
}

void RedisBaseServer::unlock() {
    pthread_mutex_unlock(&t);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

RedisServer::RedisServer() {
    this->c = nullptr;
    pthread_mutex_init(&this->t, NULL);
}

RedisServer::~RedisServer() {
    if (this->c != NULL) {
        redisFree(this->c);
        this->c = NULL;
    }
    pthread_mutex_destroy(&this->t);
}

int RedisServer::start(const char *ip, int port, int db) {
    struct timeval timeout = { 2, 500000 }; // 2.5 seconds
    this->c = redisConnectWithTimeout(ip, port, timeout);
    if (this->c == NULL || this->c->err) {
        if (this->c) {
            printf("connection error: %s\n", this->c->errstr);
            if (this->c != NULL) {
                redisFree(this->c);
                this->c = NULL;
            }
        } else {
            printf("connection error: can't allocate redis context\n");
        }
        return -1;
    }
    
    long long r = this->select(db);
    if (r == 1) {
        baseCmd.setContext(this, this->c);
        stringCmd.setContext(this, this->c);
        hashCmd.setContext(this, this->c);
    }
    else {
        if (this->c != NULL) {
            redisFree(this->c);
            this->c = NULL;
        }
        printf("select db error.[%d]\n", db);
        return -1;
    }
    
    return 0;
}

long long RedisServer::select(int db) {
    if (db < 0) {
        db = 0;
    }
    
    redisReply* reply;
    {
        this->lock();
        reply = (redisReply *)redisCommand(this->c, "SELECT %d", db);
        this->unlock();
    }
    
    long long r = 0;
    if (reply) {
        if (strcmp(reply->str, "OK") == 0) {
            r = 1;
        }
        else {
            printf("%s\n", reply->str);
        }
    }
    freeReplyObject(reply);
    
    return r;
}

BaseCommand* RedisServer::baseCommand() {
    return &this->baseCmd;;
}

StringCommand* RedisServer::stringCommand() {
    return &this->stringCmd;
}

HashCommand* RedisServer::hashCommand() {
    return &this->hashCmd;
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

