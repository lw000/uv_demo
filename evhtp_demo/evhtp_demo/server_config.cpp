//
//  server_config.cpp
//  evhtp_demo
//
//  Created by 李伟 on 2018/8/16.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include "server_config.hpp"

#include <string>
#include <iostream>
#include <fstream>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/reader.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/filereadstream.h>

std::map<std::string, std::string> load_config(const std::string& filename) {
    std::map<std::string, std::string> result;
    do {
        {
            std::ofstream out;
            out.open("./a.txt");
            out << "随机序列号" << std::endl;
            for (int i = 0; i < 100; i++) {
                out << i << std::endl;
            }
            out.flush();
            out.close();
            
            std::ifstream in;
            in.open("./a.txt");
            std::string str;
            in >> str;
            std::cout << str << std::endl;
            in.close();
        }
        
        FILE *f = NULL;
        f = fopen(filename.c_str(), "r");
        fseek(f, 0, SEEK_END);
        long long len = ftell(f);
        fseek(f, 0, SEEK_SET);
        std::shared_ptr<char> buff(new char(len));
        //char buff[2048];
        rapidjson::Document jsonRoot;
        rapidjson::FileReadStream is(f, buff.get(), len);
        jsonRoot.ParseStream<0>(is);
        if (jsonRoot.HasParseError()) {
            fclose(f);
            break;
        }
        
        fclose(f);

        result["platform"] = jsonRoot["platform"].GetString();
        result["host"] = jsonRoot["host"].GetString();
        result["port"] = jsonRoot["port"].GetString();
        
    } while(0);
    
    return result;
}

