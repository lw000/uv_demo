//
//  login_server.cpp
//  rpclib_demo
//
//  Created by 李伟 on 2018/8/9.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include "login_server.hpp"
#include "redis_server.hpp"

BaseServer::BaseServer() {
    
}

BaseServer::~BaseServer() {
    
}

LoginServer::LoginServer(rpc::server* srv, RedisServer* redisServer) {
    this->redisServer = redisServer;
    
    srv->bind("loginserver/register", [this](const std::string &name, const std::string &psd){
        return this->uregister(name, psd);
    });
    
    srv->bind("loginserver/login", [this](const std::string &uid, const std::string &psd){
        return this->ulogin(uid, psd);
    });
    
    srv->bind("loginserver/logout", [this](const std::string &uid){
        return this->ulogout(uid);
    });
}

LoginServer::~LoginServer() {
    
}

std::string LoginServer::uregister(const std::string& name, const std::string& psd) {
    if (name.empty()) {
        return "";
    }
    
    if (psd.empty()) {
        return "";
    }
    
    long long incr = this->redisServer->incr("autoincr");
    long long incr1 = this->redisServer->incrby("autoincr_by", 10000);
    double incr2 = this->redisServer->incrfloat("autoincrbyfloat", 0.2f);
    printf("incr: %lld\n", incr);
    printf("incrby: %lld\n", incr1);
    printf("incrfloat: %f\n", incr2);
    std::string value = this->redisServer->getValue("user:", name);
    
    return "";
}

std::string LoginServer::ulogin(const std::string& uid, const std::string& psd) {
    if (uid.empty()) {
        return "";
    }
    
    if (psd.empty()) {
        return "";
    }
    
    std::string value = this->redisServer->getValue("user:", uid);
    
    return "";
}

int LoginServer::ulogout(const std::string& uid) {
    if (uid.empty()) {
        return 0;
    }
    
    return 0;
}
