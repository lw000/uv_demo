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

#include "data.hpp"

#define PORT 6789

int client_run(int argc, const char * argv[]) {
    rpc::client cli("127.0.0.1", PORT);
    
    int execount = 10000;
    
    clock_t t = clock();
    //    try {
    //        {
    //            auto result = cli.async_call("add", 2, 3);
    //            int c = result.get().as<int>();
    //            printf("add: %d\n", c);
    //        }
    //
    //        {
    //            auto result = cli.async_call("mul", 2, 3);
    //            int c = result.get().as<int>();
    //            printf("mul: %d\n", c);
    //        }
    //
    //        {
    //            auto result = cli.async_call("sub", 2, 3);
    //            int c = result.get().as<int>();
    //            printf("sub: %d\n", c);
    //        }
    //
    //    } catch (rpc::rpc_error &e) {
    //        printf("%s\n", e.what());
    //    }
    
    {
        try {
            for (int i = 0; i < execount; i++) {
                {
                    std::string uid = cli.call("loginserver/register", "13632767233", "liwei", "123456").as<std::string>();
                    printf("loginserver/register: %s\n", uid.c_str());
//                    rpc_login_result_reponse reponse;
//                    if (reponse.decode(data)) {
//                    }
                    
                    std::string s = cli.call("loginserver/login", uid, "123456").as<std::string>();
                    std::string s1 = cli.call("loginserver/logout", uid).as<std::string>();
                }
                
                {
                    std::string uid = cli.call("loginserver/register", "13632767288", "liwei", "123456").as<std::string>();
                    printf("loginserver/register: %s\n", uid.c_str());
                    std::string s = cli.call("loginserver/login", uid, "123456").as<std::string>();
                    std::string s1 = cli.call("loginserver/logout", uid).as<std::string>();
                }
                
                {
                    std::string uid = cli.call("loginserver/register", "13632558737", "heshanshan", "123456").as<std::string>();
                    printf("loginserver/register: %s\n", uid.c_str());
                    std::string s = cli.call("loginserver/login", uid, "123456").as<std::string>();
                    std::string s1 = cli.call("loginserver/logout", uid).as<std::string>();
                }
                
                {
                    cli.async_call("test");
                }
            }
        } catch (rpc::rpc_error &e) {
            printf("%s\n", e.what());
        }
    }
    
    //    std::string s("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
    //    for (int i = 0; i < execount; i++) {
    //        std::string key;
    //        std::random_shuffle(s.begin(), s.end(), my_random);
    //        key.insert(key.begin(), s.begin(), s.end());
    //
    //        std::string c3 = cli.call("getUserInfo", key).as<std::string>();
    //        printf("[%4d] getUserInfo: %s\n", i, c3.c_str());
    //        std::string c4 = cli.call("ok").as<std::string>();
    //        printf("[%4d] ok: %s\n", i, c4.c_str());
    //
    //        std::string c5 = cli.call("get_time").as<std::string>();
    //        printf("[%4d] get_time: %s\n", i, c5.c_str());
    //    }
    
    clock_t t1 = clock();
    printf("exec:[%d], times:[%f]\n", execount, ((double)t1-t)/CLOCKS_PER_SEC);
    
    return 0;
}
