//
//  login_server.hpp
//  rpclib_demo
//
//  Created by 李伟 on 2018/8/9.
//  Copyright © 2018年 李伟. All rights reserved.
//

#ifndef login_server_hpp
#define login_server_hpp

#include <stdio.h>
#include <string>
#include <sstream>
#include <rpc/server.h>
#include "rpc_core_protocol.hpp"

class RedisServer;

class BaseServer {
public:
    BaseServer();
    virtual ~BaseServer();
    
protected:
    std::string tag;
    RedisServer* redisServer;
};

class LoginServer : public BaseServer {
public:
    LoginServer(rpc::server* srv, RedisServer* redisServer);
    virtual ~LoginServer();
    
public:
    std::string uregister(const std::string& phone, const std::string& name, const std::string& psd);
    std::string uregister1(std::map<std::string, std::string>& args);
//    std::string uregister2(const rpc_register_request& args);
    
    std::string ulogin(const std::string& uid, const std::string& psd);
    std::string ulogout(const std::string& uid);
    
public:
    std::string getUserInfo(const std::string& uid);
};

#endif /* login_server_hpp */
