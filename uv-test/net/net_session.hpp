//
//  Session.hpp
//  uv-test
//
//  Created by 李伟 on 2018/8/7.
//  Copyright © 2018年 李伟. All rights reserved.
//

#ifndef Session_hpp
#define Session_hpp

#include <stdio.h>

#include "uv.h"

class Session {
private:
    uv_tcp_t* t;
    
public:
    Session();
    ~Session();
    
public:
    int createSession(uv_loop_t* loop);
};

#endif /* Session_hpp */
