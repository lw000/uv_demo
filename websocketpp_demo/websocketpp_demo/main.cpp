//
//  main.cpp
//  websocketpp_demo
//
//  Created by 李伟 on 2018/8/15.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "echo.h"

int main(int argc, const char * argv[]) {
    if (strcmp(argv[1], "-s") == 0) {
        echo_server_main(argc, argv);
    } else if (strcmp(argv[1], "-c") == 0) {
        echo_client_main(argc, argv);
    } else {
        
    }
    
    return 0;
}
