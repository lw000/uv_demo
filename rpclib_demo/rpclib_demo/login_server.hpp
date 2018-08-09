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
#include <rpc/server.h>

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
    std::string uregister(const std::string& name, const std::string& psd);
    std::string ulogin(const std::string& uid, const std::string& psd);
    int ulogout(const std::string& uid);
};

#endif /* login_server_hpp */
