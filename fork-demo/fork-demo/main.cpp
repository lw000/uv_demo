//
//  main.cpp
//  fork-demo
//
//  Created by 李伟 on 2018/8/6.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include <iostream>
#include <unistd.h>

int main(int argc, const char * argv[]) {
    // insert code here...
    pid_t fpid;
    int count = 0;
    fpid = fork();
    if (fpid < 0) {
        printf("error in fork.\n");
    } else if(fpid == 0) {
        printf("i am the child process, my process id is %d \n", getpid());
        printf("我是我爹的儿子\n");
        count++;
    } else {
        printf("i am the parent process, my process id is %d \n", getpid());
        printf("我是我孩子的爹\n");
        count++;
    }
    printf("统计结果: %d\n", count);
    return 0;
}
