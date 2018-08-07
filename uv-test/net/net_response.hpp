//
//  Response.hpp
//  uv-test
//
//  Created by 李伟 on 2018/8/7.
//  Copyright © 2018年 李伟. All rights reserved.
//

#ifndef Response_hpp
#define Response_hpp

#include <stdio.h>
#include <string>
#include <map>
#include <vector>

class Reponse {
private:
    std::string version;
    int status;
    std::string description;
    std::map<std::string, std::string> kvs;
    std::vector<char> body;
    
public:
    Reponse();
    ~Reponse();
    
public:
    int setBody(const std::string& body);
};

#endif /* Response_hpp */
