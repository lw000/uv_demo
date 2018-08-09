//
//  main.cpp
//  rpclib_demo
//
//  Created by 李伟 on 2018/8/7.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include <iostream>
#include "redis_server.hpp"
#include "login_server.hpp"

#include "client_test.hpp"
#include "server_test.hpp"

//void sign_handler(int signum) {
//    printf("signnum: %d", signum);
//    exit(0);
//}

int main(int argc, const char * argv[]) {
    
//    ::signal(SIGINT|SIGTERM, sign_handler);
    
    if (strcmp(argv[1], "-s") == 0) {
        server_run(argc, argv);
    } else if (strcmp(argv[1], "-c") == 0) {
        client_run(argc, argv);
    } else {
        
    }
}
