//
//  login_server.cpp
//  rpclib_demo
//
//  Created by 李伟 on 2018/8/9.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include "login_server.hpp"
#include "redis_server.hpp"
#include "memory_redis_mapping_datastruct.hpp"

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
BaseServer::BaseServer() {
    
}

BaseServer::~BaseServer() {
    
}

LoginServer::LoginServer(rpc::server* srv, RedisServer* redisServer) {
    this->redisServer = redisServer;
    
    long long c = this->redisServer->stringCommand()->setnx("autoincr", "1000000");
    printf("init autoincr. [%d]\n", c);
    
    srv->bind("loginserver/register", [this](const std::string& phone, const std::string &name, const std::string &psd){
        return this->uregister(phone, name, psd);
    });
    
    srv->bind("loginserver/login", [this](const std::string &uid, const std::string &psd){
        return this->ulogin(uid, psd);
    });
    
    srv->bind("loginserver/logout", [this](const std::string &uid){
        return this->ulogout(uid);
    });
    
    srv->bind("loginserver/getUserInfo", [this](const std::string &uid){
        return this->getUserInfo(uid);
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
        std::map<std::string,std::string> userinfo = this->redisServer->hashCommand()->hgetall(phone, "user_infos:");
        if (userinfo.empty()) {
//            {
//                long long incr = this->redisServer->stringCommand()->incr("autoincr");
//                long long incr1 = this->redisServer->stringCommand()->incrby("autoincr_by", 10);
//                double incr2 = this->redisServer->stringCommand()->incrfloat("autoincrbyfloat", 0.2f);
//            }
            long long uid = this->redisServer->stringCommand()->incr("autoincr");
            char uid_buf[64] = {0};
            sprintf(uid_buf, "%lld", uid);
            
            UserInfo uinfo;
            uinfo.phone = phone;
            uinfo.name = name;
            uinfo.psd = psd;
            uinfo.uid = uid_buf;
            
            long long ok = -1;
            ok = this->redisServer->hashCommand()->hset(phone, "phone", phone, "user_infos:");
            ok = this->redisServer->hashCommand()->hset(phone, "name", name, "user_infos:");
            ok = this->redisServer->hashCommand()->hset(phone, "psd", psd, "user_infos:");
            ok = this->redisServer->hashCommand()->hset(phone, "uid", uid_buf, "user_infos:");
            
//            ok = this->redisServer->hashCommand()->hmset(phone, "field liwei", "user_infos:");
            
            if (ok == 1) {
                ok = this->redisServer->hashCommand()->hset(uid_buf, "phone", phone, "user_uid:");
                if (ok == 1) {
//                    rapidjson::Document doc;
//                    doc.SetObject();
//                    rapidjson::Document::AllocatorType& alloctor = doc.GetAllocator();
//                    doc.AddMember("code", 0, alloctor);
//                    doc.AddMember("what", "ok", alloctor);
//                    {
//                        rapidjson::Value data;
//                        data.SetObject();
//                        data.AddMember("uid", uinfo.uid.c_str(), alloctor);
//                        data.AddMember("session", "", alloctor);
//                        doc.AddMember("data", data, alloctor);
//                    }
//                    rapidjson::StringBuffer buffer;
//                    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
//                    doc.Accept(writer);
//                    std::string jsondst = buffer.GetString();
//                    return jsondst;
                    return uinfo.uid;
                }
                else {
                    printf("update user_infos cache error.\n");
                }
            }
            else {
                printf("update user_uid cache error.\n");
            }
        }
        else {
//            rapidjson::Document doc;
//            doc.SetObject();
//            rapidjson::Document::AllocatorType& alloctor = doc.GetAllocator();
//            doc.AddMember("code", 0, alloctor);
//            doc.AddMember("what", "ok", alloctor);
//            {
//                rapidjson::Value data;
//                data.SetObject();
//                data.AddMember("uid", userinfo["uid"].c_str(), alloctor);
//                data.AddMember("session", "", alloctor);
//                doc.AddMember("data", data, alloctor);
//            }
//
//            rapidjson::StringBuffer buffer;
//            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
//            doc.Accept(writer);
//            std::string jsondst = buffer.GetString();
//            return jsondst;
            
            return userinfo["uid"];
        }
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
    
    return "";
}

std::string LoginServer::ulogout(const std::string& uid) {
    if (uid.empty()) {
        return "";
    }
    
    return "";
}

std::string LoginServer::getUserInfo(const std::string& uid) {
    if (uid.empty()) {
        return "";
    }
    
    std::string phone = this->redisServer->hashCommand()->hget(uid, "phone", "user_uid:");
    std::map<std::string,std::string> userinfo = this->redisServer->hashCommand()->hgetall(phone, "user_infos:");
    
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& alloctor = doc.GetAllocator();
    doc.AddMember("code", 0, alloctor);
    doc.AddMember("what", "ok", alloctor);
    rapidjson::Value data;
    data.SetObject();
    for (auto& u : userinfo) {
        rapidjson::Value k;
        k.SetString(u.first.c_str(), u.first.size(), alloctor);
        rapidjson::Value v;
        v.SetString(u.second.c_str(), u.second.size(), alloctor);
        data.AddMember(v, k, alloctor);
    }
    doc.AddMember("data", data, alloctor);
    
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    std::string jsondst = buffer.GetString();
    return jsondst;
}
