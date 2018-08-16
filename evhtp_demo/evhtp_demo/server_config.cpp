//
//  server_config.cpp
//  evhtp_demo
//
//  Created by 李伟 on 2018/8/16.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include "server_config.hpp"

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/reader.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/filereadstream.h>

std::map<std::string, std::string> load_config(const std::string& filename) {
    
    std::map<std::string, std::string> result;
    do {
        FILE *f = NULL;
        f = fopen(filename.c_str(), "r");
        fseek(f, 0, SEEK_SET);
        char buff[2048];
        rapidjson::Document root;
        rapidjson::FileReadStream is(f, buff, sizeof(buff));
        root.ParseStream<0>(is);
        if (root.HasParseError()) {
            fclose(f);
            break;
        }
        fclose(f);

        result["platform"] = root["platform"].GetString();
        result["host"] = root["host"].GetString();
        result["port"] = root["port"].GetString();
    } while(0);
    
    return result;
}

