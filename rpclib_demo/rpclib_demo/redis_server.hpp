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

int start_redis_main_server(int argc, const char * argv[]);

class RedisServer {
public:
    RedisServer();
    ~RedisServer();
    
public:
    int start();
    
public:
    void onConnect(int status);
    void onDisconnect(int status);
};
    
#endif /* redis_server_hpp */
