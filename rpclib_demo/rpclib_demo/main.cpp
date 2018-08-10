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

int32_t JumpConsistentHash(uint64_t key, int32_t num_buckets)
{
    int64_t b = 1, j = 0;
    while (j < num_buckets)
    {
        b = j;
        key = key * 2862933555777941757ULL + 1;
        j = (b + 1) * (double(1LL << 31) / double((key >> 33) + 1));
    }
    
    return b;
}

int main(int argc, const char * argv[]) {
    
//    ::signal(SIGINT|SIGTERM, sign_handler);
    
    if (strcmp(argv[1], "-s") == 0) {
        server_run(argc, argv);
    } else if (strcmp(argv[1], "-c") == 0) {
        client_run(argc, argv);
    } else {
        
    }
}
