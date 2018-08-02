//
//  main_go.cpp
//  mill
//
//  Created by 李伟 on 2018/7/26.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include <errno.h>
#include <assert.h>
#include <stdio.h>
#include <sys/wait.h>

#include "libmill.h"

int sum = 0;

coroutine void worker(int count, int n) {
    int i;
    for(i = 0; i != count; ++i) {
        sum += n;
        yield();
        printf("sum: %d\n", sum);
    }
}

coroutine void dummy(void) {
    msleep(now() + 50);
}

#define TEST

#ifdef TEST
int main(int argc, const char * argv[]) {
#else
int main_go(int argc, const char * argv[]) {
#endif
    
    goprepare(10, 25000, 300);
    
//    goprepare(1, 10, 5);
    
    /* Try few coroutines with pre-prepared stacks. */
    assert(errno == 0);
    go(worker(3, 7));
    go(worker(1, 11));
    go(worker(2, 5));
    msleep(100);
    assert(sum == 42);
    
    /* Test whether stack deallocation works. */
    int i;
    for(i = 0; i != 20; ++i)
        go(dummy());
    msleep(now() + 100);
    
    /* Try to fork the process. */
//    pid_t pid = mfork();
//    assert(pid != -1);
//    if(pid > 0) {
//        int status;
//        pid = waitpid(pid, &status, 0);
//        assert(pid != -1);
//        assert(WIFEXITED(status));
//    }
    
    return 0;
}


