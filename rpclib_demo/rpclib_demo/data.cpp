//
//  data.cpp
//  rpclib_demo
//
//  Created by 李伟 on 2018/8/9.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include "data.hpp"

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

int _rpc_login_result_reponse::decode(const std::string& data) {
//    std::ostringstream out;
//    return out.str();
    
    std::string uid;
    
    rapidjson::Document doc;
    doc.Parse<0>(data.c_str());
    if (doc.HasParseError()) {
        return -1;
    }
    
    this->code = doc["code"].GetInt();
    this->what = doc["what"].GetString();
    this->uid = doc["data"]["uid"].GetString();
    this->session = doc["data"]["session"].GetString();
    
    return 0;
}
