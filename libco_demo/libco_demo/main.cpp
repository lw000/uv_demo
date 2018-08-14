//
//  main.cpp
//  libco_demo
//
//  Created by 李伟 on 2018/8/14.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>

#include "co_routine.h"

#include "echosvr.hpp"
#include "echocli.hpp"

static int loop(void *)
{
    return 0;
}

static void *routine_func( void * )
{
    stCoEpoll_t * ev = co_get_epoll_ct(); //ct = current thread
    co_eventloop( ev, loop, 0);
    
    return 0;
}

int main(int argc, char * argv[]) {
    
//    if (argc < 2) {
//        printf("please input -s or -c\n");
//        return 0;
//    }
//
//    if (strcmp(argv[1], "-s") == 0) {
//        echo_srv_main(argc, argv);
//    } else if(strcmp(argv[1], "-c") == 0) {
//        echo_cli_main(argc, argv);
//    } else {
//        printf("please input -s or -c\n");
//    }

    //int cnt = atoi(argv[1]);
    
    int cnt = 2;
    
    pthread_t tid[cnt];
    for(int i = 0; i < cnt; i++)
    {
        pthread_create(tid + i, NULL, routine_func, 0);
    }
    for(;;)
    {
        sleep(1);
    }
    
    return 0;
}
