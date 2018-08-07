//
//  main.cpp
//  rpclib_demo
//
//  Created by 李伟 on 2018/8/7.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include <iostream>

#include <rpc/server.h>
#include <rpc/client.h>

#define PORT 6789

static void bad(int x) {
    if (x == 5) {
        throw std::runtime_error("x == 5. I really don't like 5.");
    }
}

struct Sub {
    int operator()(int a, int b) {
        return a-b;
    }
};


int client_run(int argc, const char * argv[]) {
    
    rpc::client cli("127.0.0.1", PORT);
    
    int c = cli.call("add", 2, 3).as<int>();
    printf("add: %d\n", c);
    int c1 = cli.call("mul", 2, 3).as<int>();
    printf("mul: %d\n", c1);
    int c2 = cli.call("sub", 2, 3).as<int>();
    printf("sub: %d\n", c2);
    
    return 0;
}

int server_run(int argc, const char * argv[]) {
    
    rpc::server srv("0.0.0.0", PORT != 0 ? PORT : rpc::constants::DEFAULT_PORT);
    
    srv.bind("add", [](int a, int b) { return a + b; });
    srv.bind("mul", [](int a, int b) { return a * b; });
    srv.bind("bad", &bad);
    Sub sub;
    srv.bind("sub", sub);
    
    srv.run();
    
    return 0;
}

int main(int argc, const char * argv[]) {
    if (strcmp(argv[1], "-s") == 0) {
        server_run(argc, argv);
    } else if (strcmp(argv[1], "-c") == 0) {
        client_run(argc, argv);
    } else {
        
    }
}
