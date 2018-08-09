//
//  login_server.cpp
//  rpclib_demo
//
//  Created by 李伟 on 2018/8/9.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include "login_server.hpp"
#include "redis_server.hpp"

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

typedef struct _UserInfo {
    std::string phone;
    std::string name;
    std::string psd;
    std::string uid;
    
public:
    _UserInfo() {
        
    }
    
public:
    std::string encode() {
        std::ostringstream out;
        out << " phone " << phone << " name " << name << " psd " << psd << " uid " << uid;
        return out.str();
    }
} UserInfo;

////////////////////////////////////////////////////////////////////////////////////////////////////
BaseServer::BaseServer() {
    
}

BaseServer::~BaseServer() {
    
}

LoginServer::LoginServer(rpc::server* srv, RedisServer* redisServer) {
    this->redisServer = redisServer;
    
    srv->bind("loginserver/register", [this](const std::string& phone, const std::string &name, const std::string &psd){
        return this->uregister(phone, name, psd);
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

std::string LoginServer::uregister(const std::string& phone, const std::string& name, const std::string& psd) {
    if (phone.empty()) {
        return "";
    }
    
    if (name.empty()) {
        return "";
    }
    
    if (psd.empty()) {
        return "";
    }

    {
        std::map<std::string,std::string> userinfo = this->redisServer->hashCommand()->hgetall(phone, "users:");
        if (userinfo.empty()) {
//            {
//                long long incr = redisCache.baseCommand()->incr("autoincr");
//                long long incr1 = redisCache.baseCommand()->incrby("autoincr_by", 10);
//                double incr2 = redisCache.baseCommand()->incrfloat("autoincrbyfloat", 0.2f);
//            }

            long long uid = this->redisServer->baseCommand()->incr("autoincr");
            char uid_buffer[64] = {0};
            sprintf(uid_buffer, "%lld", uid);
            
            UserInfo uinfo;
            uinfo.phone = phone;
            uinfo.name = name;
            uinfo.psd = psd;
            uinfo.uid = uid_buffer;
            std::string cmd = uinfo.encode();
            
            long long ok = -1;
            ok = this->redisServer->hashCommand()->hset(phone, "phone", phone, "users:");
            ok = this->redisServer->hashCommand()->hset(phone, "name", name, "users:");
            ok = this->redisServer->hashCommand()->hset(phone, "psd", psd, "users:");
            ok = this->redisServer->hashCommand()->hset(phone, "uid", uid_buffer, "users:");
        }
        
        rapidjson::Document doc;
        doc.SetObject();
        rapidjson::Document::AllocatorType& alloctor = doc.GetAllocator();
        doc.AddMember("code", 0, alloctor);
        doc.AddMember("what", "ok", alloctor);
        {
            rapidjson::Value data;
            data.SetObject();
            data.AddMember("uid", userinfo["uid"].c_str(), alloctor);
            data.AddMember("session", "", alloctor);
            doc.AddMember("data", data, alloctor);
        }
        
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        doc.Accept(writer);
        std::string jsondst = buffer.GetString();
        return jsondst;
    }
    
    return "";
}

std::string LoginServer::ulogin(const std::string& uid, const std::string& psd) {
    if (uid.empty()) {
        return "";
    }
    
    if (psd.empty()) {
        return "";
    }
    
    std::string value = this->redisServer->baseCommand()->get("user:", uid);
    
    return "";
}

int LoginServer::ulogout(const std::string& uid) {
    if (uid.empty()) {
        return 0;
    }
    
    return 0;
}
