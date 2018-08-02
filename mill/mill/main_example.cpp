//
//  main_example.c
//  mill
//
//  Created by 李伟 on 2018/7/26.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include <stdio.h>
#include "libmill.h"

coroutine void worker(int count, const char *text) {
    int i;
    for(i = 0; i != count; ++i) {
        printf("%s\n", text);
        msleep(now() + 10);
    }
}

//#define TEST

#ifdef TEST
int main(int argc, const char * argv[]) {
#else
int main_example(int argc, const char * argv[]) {
#endif
        
    go(worker(4, "a"));
    go(worker(2, "b"));
    go(worker(3, "c"));
    
    msleep(now () + 100);
    
    return 0;
}
