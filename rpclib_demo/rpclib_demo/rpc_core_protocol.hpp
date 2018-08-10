//
//  data.hpp
//  rpclib_demo
//
//  Created by 李伟 on 2018/8/9.
//  Copyright © 2018年 李伟. All rights reserved.
//

#ifndef data_hpp
#define data_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>

typedef struct _rpc_base_reponse {
    int code;
    std::string what;
} rpc_base_reponse;

typedef struct _rpc_login_result_reponse : public _rpc_base_reponse {
    std::string uid;
    std::string session;
    
public:
    int decode(const std::string& data);
    
} rpc_login_result_reponse;

#endif /* data_hpp */
