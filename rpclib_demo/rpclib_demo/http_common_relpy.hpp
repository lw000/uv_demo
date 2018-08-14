//
//  http_common_relpy.hpp
//  rpclib_demo
//
//  Created by 李伟 on 2018/8/14.
//  Copyright © 2018年 李伟. All rights reserved.
//

#ifndef http_common_relpy_hpp
#define http_common_relpy_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <map>

std::string reply_json(int code, const std::string& what, const std::map<std::string, std::string>& dataMap);
    
#endif /* http_common_relpy_hpp */
