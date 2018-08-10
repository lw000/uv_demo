//
//  memory_redis_mapping_datastruct.hpp
//  rpclib_demo
//
//  Created by 李伟 on 2018/8/10.
//  Copyright © 2018年 李伟. All rights reserved.
//

#ifndef memory_redis_mapping_datastruct_hpp
#define memory_redis_mapping_datastruct_hpp

#include <stdio.h>
#include <string>
#include <map>

typedef struct _UserInfo {
    std::string phone;
    std::string name;
    std::string psd;
    std::string uid;
    
public:
    _UserInfo();
    
public:
    std::string encode_string_redis_command();
    std::map<std::string, std::string> encode_map_redis_command();
    std::string encode_json();
    
} UserInfo;

#endif /* memory_redis_mapping_datastruct_hpp */
