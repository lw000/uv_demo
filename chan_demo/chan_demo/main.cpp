//
//  main.cpp
//  chan_demo
//
//  Created by 李伟 on 2018/8/15.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <pthread.h>
#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif
    #include "./chan/chan.h"
#ifdef __cplusplus
}
#endif

chan_t* chan;

class AAA : public std::enable_shared_from_this<AAA> {
public:
    std::shared_ptr<AAA> getptr() {
        return shared_from_this();
    }
    
    ~AAA() {
        std::cout << "AAA call" << std::endl;
    }
};

class BBB {
public:
    ~BBB() {
        std::cout << "BBB call" << std::endl;
    }
    
    void pf() {
        std::cout << "pf call" << std::endl;
    }
};

void* ping(void*)
{
    // Send blocks until receiver is ready.
    for (int i = 0; i < 10000; i++) {
        chan_send(chan, (void*)"ping");
    }
    
    return NULL;
}

void* ping1(void*)
{
    // Send blocks until receiver is ready.
    for (int i = 0; i < 10000; i++) {
        chan_send(chan, (void*)"ping1");
    }
    chan_send(chan, (void*)"quit");
    
    return NULL;
}

int main(int argc, const char * argv[]) {
    
    {
        std::shared_ptr<AAA> a(new AAA());
        std::shared_ptr<AAA> b = a->getptr();
        std::cout << "a use_count: " << a.use_count() << std::endl;
        std::cout << "b use_count: " << b.use_count() << std::endl;
    }
    
    {
        std::shared_ptr<BBB> b(new BBB());
        b->pf();
    }
    
    return 0;
    
    // Initialize unbuffered channel.
    chan = chan_init(1024);
    
    {
        pthread_t th;
        pthread_create(&th, NULL, ping, NULL);
    }

    {
        pthread_t th;
        pthread_create(&th, NULL, ping1, NULL);
    }

    // Receive blocks until sender is ready.
    
    void* msg;
    while (chan_recv(chan, &msg) == 0) {
        if (strcmp((char*)msg, "quit") == 0) {
            break;
        }
        
        if (strcmp((char*)msg, "ping") == 0) {
            printf("ping: %s\n", msg);
        } else if (strcmp((char*)msg, "ping1") == 0) {
            printf("ping1: %s\n", msg);
        }
        else {
            printf("%s\n", msg);
        }
    }
    printf("received all jobs\n");
    
    // Clean up channel.
    chan_dispose(chan);
    
    return 0;
}
