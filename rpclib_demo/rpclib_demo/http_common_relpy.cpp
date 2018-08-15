//
//  http_common_relpy.cpp
//  rpclib_demo
//
//  Created by 李伟 on 2018/8/14.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include "http_common_relpy.hpp"

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include <nlohmann/json.hpp>

std::string reply_json(int code, const std::string& what, const std::map<std::string, std::string>& dataMap) {
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    doc.AddMember("code", code, allocator);
    doc.AddMember("what", rapidjson::Value(what.c_str(), (unsigned)what.size()), allocator);
    rapidjson::Value data;
    data.SetObject();
    {
        for (auto &m : dataMap) {
            rapidjson::Value k;
            k.SetString(m.first.c_str(), (unsigned)m.first.size());
            rapidjson::Value v;
            v.SetString(m.second.c_str(), (unsigned)m.second.size());
            data.AddMember(k, v, allocator);
        }
        doc.AddMember("data", data, allocator);
    }
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    std::string result = buffer.GetString();
    return result;
}
