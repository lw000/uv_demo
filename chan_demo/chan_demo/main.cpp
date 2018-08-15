//
//  main.cpp
//  chan_demo
//
//  Created by 李伟 on 2018/8/15.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif
    
    #include "./chan/chan.h"
    
#ifdef __cplusplus
}
#endif

chan_t* chan;

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
    while (1) {
        void* msg;
        chan_recv(chan, &msg);
        if (strcmp((char*)msg, "quit") == 0) {
            break;
        } else if (strcmp((char*)msg, "ping") == 0) {
            printf("ping: %s\n", msg);
        } else if (strcmp((char*)msg, "ping1") == 0) {
            printf("ping1: %s\n", msg);
        }
        else {
            printf("%s\n", msg);
        }
    }

    // Clean up channel.
    chan_dispose(chan);
    
    return 0;
}
