//
//  server_test.cpp
//  rpclib_demo
//
//  Created by 李伟 on 2018/8/9.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include "server_test.hpp"

#include <iostream>
#include <unistd.h>
#include <rpc/server.h>
#include <rpc/this_handler.h>
#include <rpc/this_session.h>
#include <rpc/rpc_error.h>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include "redis_server.hpp"
#include "login_server.hpp"
#include "rpc_core_protocol.hpp"

#include "client_test.hpp"

#define PORT 6789

RedisServer redisCache;

static void bad(int x) {
    if (x == 5) {
        throw std::runtime_error("x == 5. I really don't like 5.");
    }
}

static double divide(double a, double b) {
    if (b > -0.00000001 && b < 0.00000001) {
        rpc::this_handler().respond_error("Division by zero!");
    }
    return a / b;
}

static std::string getUserInfo(const std::string& name) {
    // search cache info
    std::string value = redisCache.stringCommand()->get(name, "name:");
    if (!value.empty()) {
        return value;
    }
    
    //        rpc::this_session().post_exit();
    
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& alloctor = doc.GetAllocator();
    doc.AddMember("code", 0, alloctor);
    doc.AddMember("what", "ok", alloctor);
    {
        rapidjson::Value data;
        data.SetObject();
        data.AddMember("name", name.c_str(), alloctor);
        data.AddMember("address", "shenzhenshinanshanqu", alloctor);
        doc.AddMember("data", data, alloctor);
    }
    
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    std::string jsondst = buffer.GetString();
    
    long long c = redisCache.stringCommand()->set(name, jsondst, "name:");
    if (c == 0) {
        
        /*
         update mysql db
         */
        
        return jsondst;
    }
    
    printf("update cache error. [%lld]\n", c);
    
    {
        rapidjson::Document doc;
        doc.SetObject();
        rapidjson::Document::AllocatorType& alloctor = doc.GetAllocator();
        doc.AddMember("code", 0, alloctor);
        doc.AddMember("what", "update cache error.", alloctor);
        {
            rapidjson::Value data;
            data.SetObject();
            doc.AddMember("data", data, alloctor);
        }
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        doc.Accept(writer);
        std::string errorjson = buffer.GetString();
        return errorjson;
    }
}

static int my_random(int c) {
    return std::rand() % c;
}

typedef struct {
    int operator() (int a, int b) {
        return a-b;
    }
    
    std::string ok() {
        return "1234567890";
    }
} Sub;

class generator_random {
public:
    ptrdiff_t operator ()(ptrdiff_t max) {
        double t;
        t = static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX);
        return static_cast<ptrdiff_t>(t * max);
    }
};

int server_run(int argc, const char * argv[]) {
    rpc::server srv("0.0.0.0", PORT != 0 ? PORT : rpc::constants::DEFAULT_PORT);
    srv.suppress_exceptions(true);
    
    srv.bind("add", [](int a, int b) { return a + b; });
    srv.bind("mul", [](int a, int b) { return a * b; });
    srv.bind("bad", &bad);
    srv.bind("divide", &divide);
    srv.bind("get_time", []{
        time_t rawtime;
        struct tm *timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        return asctime(timeinfo);
    });
    
    Sub sub;
    srv.bind("sub", sub);
    srv.bind("ok", [&sub]() {
        return sub.ok();
    });
    
    srv.bind("getUserInfo", &getUserInfo);
    
    srv.bind("test", []{
        {
            long long exists = redisCache.baseCommand()->exists("autoincr");
            printf("%lld", exists);
            
            long long s = redisCache.stringCommand()->setex("aaaaa", "111111111", 120);
            printf("setex: %lld\n", s);
            s = redisCache.baseCommand()->ttl("aaaaa");
            printf("ttl: %lld\n", s);
            s = redisCache.baseCommand()->pttl("aaaaa");
            printf("pttl: %lld\n", s);
        }
        
        {
//            long long incr0 = redisCache.stringCommand()->incr("autoincr");
//            long long incr1 = redisCache.stringCommand()->incrby("autoincr_by", 10000);
//            double    incr2 = redisCache.stringCommand()->incrfloat("autoincrbyfloat", 0.2f);
            
            std::map<std::string, std::string> keyvalues;
            keyvalues.insert(std::make_pair("field0", "\"liwei00\""));
            keyvalues.insert(std::make_pair("field1", "\"liwei01\""));
            keyvalues.insert(std::make_pair("field2", "\"liwei02\""));
            keyvalues.insert(std::make_pair("field3", "\"liwei03\""));
            long long s0 = redisCache.stringCommand()->mset(keyvalues);
            s0 = redisCache.stringCommand()->msetnx(keyvalues);
        }
        
        {
            long long s0 = redisCache.hashCommand()->hset("hash2", "field0", "liwei0", "hash:");
            s0 = redisCache.hashCommand()->hset("hash2", "field1", "liwei1", "hash:");
            s0 = redisCache.hashCommand()->hset("hash2", "field2", "liwei2", "hash:");
            s0 = redisCache.hashCommand()->hset("hash2", "field3", "liwei3", "hash:");
            s0 = redisCache.baseCommand()->pexpire("hash2", 2000);
            
            std::map<std::string, std::string> cmds;
            cmds.insert(std::make_pair("field0", "liwei00"));
            cmds.insert(std::make_pair("field1", "liwei01"));
            cmds.insert(std::make_pair("field2", "liwei02"));
            cmds.insert(std::make_pair("field3", "liwei03"));
            s0 = redisCache.hashCommand()->hmset("hash2", cmds, "hash:");
            std::string s6 = redisCache.hashCommand()->hget("hash2", "field0", "hash:");
            std::string s7 = redisCache.hashCommand()->hget("hash2", "field0", "hash:");
            long long s8 = redisCache.hashCommand()->hlen("hash2", "hash:");
            std::map<std::string,std::string> s9 = redisCache.hashCommand()->hgetall("hash2", "hash:");
            std::vector<std::string> s10 = redisCache.hashCommand()->hkeys("hash2", "hash:");
            bool s11 = redisCache.hashCommand()->hexists("hash2", "field1", "hash:");
        }

    });
    
    int n = redisCache.start("127.0.0.1", 6379, 1);
    if (n == 0) {
        LoginServer loginServer(&srv, &redisCache);
        
        //    srv.run();
        srv.async_run(4);
        
        while (1) {
            sleep(1);
        }
    }
    
    return 0;
}
