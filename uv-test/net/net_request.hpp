//
//  Request.hpp
//  uv-test
//
//  Created by 李伟 on 2018/8/7.
//  Copyright © 2018年 李伟. All rights reserved.
//

#ifndef Request_hpp
#define Request_hpp

#include <stdio.h>
#include <string>
#include <map>
#include <vector>

class Request {
private:
    std::string method;
    std::string url;
    std::string version;
    std::map<std::string, std::string> kvs;
    std::vector<char> body;
    
public:
    Request();
    ~Request();
    
public:
    int setBody(const std::string& body);
};

#endif /* Request_hpp */
