//
//  server_config.hpp
//  evhtp_demo
//
//  Created by 李伟 on 2018/8/16.
//  Copyright © 2018年 李伟. All rights reserved.
//

#ifndef server_config_hpp
#define server_config_hpp

#include <stdio.h>
#include <string>
#include <map>

std::map<std::string, std::string> load_config(const std::string& filename);

#endif /* server_config_hpp */
