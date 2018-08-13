//
//  memory_redis_mapping_datastruct.cpp
//  rpclib_demo
//
//  Created by 李伟 on 2018/8/10.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include "memory_redis_mapping_datastruct.hpp"

#include <sstream>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

_UserInfo::_UserInfo() {
    
}

std::string _UserInfo::encode_string_redis_command() {
    std::ostringstream out;
    out << "phone " << phone << " name " << name << " psd " << psd << " uid " << uid;
    return out.str();
}

std::map<std::string, std::string> _UserInfo::encode_map_redis_command() {
    std::map<std::string, std::string> out;
    out.insert(std::make_pair("phone", phone.c_str()));
    out.insert(std::make_pair("name", name.c_str()));
    out.insert(std::make_pair("psd", psd.c_str()));
    out.insert(std::make_pair("uid", uid.c_str()));
    return out;
}

std::string _UserInfo::encode_json() {
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& alloctor = doc.GetAllocator();
    std::map<std::string, std::string> vk = this->encode_map_redis_command();
    for(auto m : vk) {
        rapidjson::Value k;
        k.SetString(m.first.c_str(), m.first.size(), alloctor);
        rapidjson::Value v;
        v.SetString(m.second.c_str(), m.second.size(), alloctor);
        doc.AddMember(k, v, alloctor);
    }
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    std::string jsondst = buffer.GetString();
    return jsondst;
}

