//
//  main.cpp
//  rpclib_demo
//
//  Created by 李伟 on 2018/8/7.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include <rpc/server.h>
#include <rpc/client.h>
#include <rpc/this_handler.h>
#include <rpc/rpc_error.h>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#define PORT 6789

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

static struct Sub {
    int operator()(int a, int b) {
        return a-b;
    }
    
    std::string ok() {
        return "1234567890";
    }
};

int client_run(int argc, const char * argv[]) {
    rpc::client cli("127.0.0.1", PORT);
    
    int execount = 10000;
    clock_t t = clock();
    try {
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
    } catch (rpc::rpc_error &e) {
        printf("%s\n", e.what());
    }
    
    for (int i = 0; i < execount; i++) {
        std::string c3 = cli.call("getUserInfo").as<std::string>();
        printf("[%4d] getUserInfo: %s\n", i, c3.c_str());
        std::string c4 = cli.call("ok").as<std::string>();
        printf("[%4d] ok: %s\n", i, c4.c_str());
        
        std::string c5 = cli.call("get_time").as<std::string>();
        printf("[%4d] get_time: %s\n", i, c5.c_str());
    }
    clock_t t1 = clock();
    printf("exec:[%d], times:[%f]\n", execount, ((double)t1-t)/CLOCKS_PER_SEC);
    
    return 0;
}

int server_run(int argc, const char * argv[]) {
    
    rpc::server srv("0.0.0.0", PORT != 0 ? PORT : rpc::constants::DEFAULT_PORT);
    
    srv.bind("add", [](int a, int b) { return a + b; });
    srv.bind("mul", [](int a, int b) { return a * b; });
    srv.bind("bad", &bad);
    srv.bind("divide", &divide);
    srv.bind("getUserInfo", [] {
        rapidjson::Document doc;
        doc.SetObject();
        rapidjson::Document::AllocatorType& alloctor = doc.GetAllocator();
        doc.AddMember("name", "liwei", alloctor);
        doc.AddMember("address", "shenzhenshinanshanqu", alloctor);
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        doc.Accept(writer);
        std::string jsondst = buffer.GetString();
        return jsondst;
    });
    
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
    
    srv.suppress_exceptions(true);
    
//    srv.run();
    srv.async_run(6);
    
    while (1) {
        sleep(1);
    }
    
    return 0;
}

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
