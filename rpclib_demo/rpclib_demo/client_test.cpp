//
//  client_test.cpp
//  rpclib_demo
//
//  Created by 李伟 on 2018/8/9.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include "client_test.hpp"
#include <string>
#include <vector>
#include <map>

#include <rpc/client.h>
#include <rpc/this_handler.h>
#include <rpc/this_session.h>
#include <rpc/rpc_error.h>

#include "rpc_core_protocol.hpp"

#define PORT 6789

static int my_random(int c) {
    return std::rand() % c;
}

class generator_random {
public:
    ptrdiff_t operator ()(ptrdiff_t max) {
        double t;
        t = static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX);
        return static_cast<ptrdiff_t>(t * max);
    }
};

int client_run(int argc, const char * argv[]) {
    rpc::client cli("127.0.0.1", PORT);
    
    int execount = 1;
    
//    clock_t t = clock();
    
    std::string s("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
    {
        std::string key;
        std::random_shuffle(s.begin(), s.end(), my_random);
        key.insert(key.begin(), s.begin(), s.end());
    }
    
    for (int i = 0; i < execount; i++) {
//    for(;;) {
        try {
            {
                std::vector<int> v;
                v.push_back(1);
                v.push_back(2);
                v.push_back(3);
                v.push_back(4);
                v.push_back(5);
                v.push_back(6);
                v.push_back(7);
                v.push_back(8);
                v.push_back(9);
                auto result = cli.async_call("sum", v);
                int c = result.get().as<int>();
                printf("sum: %d\n", c);
            }
            
            {
                auto result = cli.async_call("add", 2, 3);
                int c = result.get().as<int>();
                printf("add: %d\n", c);
            }
        
            {
                auto result = cli.async_call("mul", 2, 3);
                int c = result.get().as<int>();
                printf("mul: %d\n", c);
            }
        
            {
                auto result = cli.async_call("sub", 2, 3);
                int c = result.get().as<int>();
                printf("sub: %d\n", c);
            }
            
            {
                std::string c5 = cli.call("get_time").as<std::string>();
                printf("get_time: %s\n", c5.c_str());
            }
            
            {
                std::map<std::string, std::string> args;
                args.insert(std::make_pair("phone", "13632767233"));
                args.insert(std::make_pair("name", "liwei"));
                args.insert(std::make_pair("psd", "123456"));
                std::string uid = cli.call("loginserver/register1", args).as<std::string>();
                printf("loginserver/register1: %s\n", uid.c_str());
            }
            
            {
                std::map<std::string, std::string> args;
                args.insert(std::make_pair("phone", "13632767233"));
                args.insert(std::make_pair("name", "liwei"));
                args.insert(std::make_pair("psd", "123456"));
                std::string uid = cli.call("loginserver/register", "13632767233", "liwei", "123456").as<std::string>();
                printf("loginserver/register: %s\n", uid.c_str());
                
//                rpc_login_result_reponse reponse;
//                if (reponse.decode(data)) {
//                }
                
                std::string s = cli.call("loginserver/login", uid, "123456").as<std::string>();
                s = cli.call("loginserver/logout", uid).as<std::string>();
                s = cli.call("loginserver/getUserInfo", uid).as<std::string>();
                printf("loginserver/getUserInfo: %s\n", s.c_str());
            }
            
            {
                std::string uid = cli.call("loginserver/register", "13632767288", "liwei", "123456").as<std::string>();
                printf("loginserver/register: %s\n", uid.c_str());
                std::string s = cli.call("loginserver/login", uid, "123456").as<std::string>();
                s = cli.call("loginserver/logout", uid).as<std::string>();
                s = cli.call("loginserver/getUserInfo", uid).as<std::string>();
                printf("loginserver/getUserInfo: %s\n", s.c_str());
            }
            
            {
                cli.call("test");
            }
            
        } catch (rpc::rpc_error &e) {
            printf("%s\n", e.what());
        }
    }
    
//    clock_t t1 = clock();
//    printf("exec:[%d], times:[%f]\n", execount, ((double)t1-t)/CLOCKS_PER_SEC);

    return 0;
}
