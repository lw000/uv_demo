//
//  net_reactor.hpp
//  uv-test
//
//  Created by 李伟 on 2018/8/3.
//  Copyright © 2018年 李伟. All rights reserved.
//

#ifndef net_reactor_hpp
#define net_reactor_hpp

#include <stdio.h>

class Reactor;
class Handler;

class Reactor {
public:
    Reactor();
    ~Reactor();
    
public:
    void run();
};


class Handler {
public:
    Handler();
    ~Handler();
};

#endif /* net_reactor_hpp */
