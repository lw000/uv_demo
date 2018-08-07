//
//  Response.cpp
//  uv-test
//
//  Created by 李伟 on 2018/8/7.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include "net_response.hpp"
#include <algorithm>

Reponse::Reponse() {
    
}

Reponse::~Reponse() {
    
}

int Reponse::setBody(const std::string& body) {
    //    for (int i = 0; i < body.size(); i++) {
    //        this->body.push_back(body[i]);
    //    }
    //
    this->body.assign(body.begin(), body.end());
    return 0;
}
