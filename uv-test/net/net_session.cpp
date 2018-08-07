//
//  Session.cpp
//  uv-test
//
//  Created by 李伟 on 2018/8/7.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include "net_session.hpp"
#include <stdlib.h>

Session::Session() {
    
}

Session::~Session() {
    
}

int Session::createSession(uv_loop_t* loop) {
    t = (uv_tcp_t*) ::malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop, t);
    return 0;
}
