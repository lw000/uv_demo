//
//  main.cpp
//  msgpack_demo
//
//  Created by 李伟 on 2018/8/7.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include <msgpack.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <map>

int main(int argc, const char * argv[]) {

    std::vector<std::string> vec;
    vec.push_back("hello");
    vec.push_back("MessagePack");
    
    std::vector<std::string> vec1;
    vec1.push_back("hello1");
    vec1.push_back("MessagePack1");
    
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, vec);
    
    msgpack::unpacked msg;
    msgpack::unpack(&msg, sbuf.data(), sbuf.size());
    
    msgpack::object obj = msg.get();
//    std::cout << obj << std::endl;
    
    std::vector<std::string> rvec;
    obj.convert(&rvec);
    
    return 0;
}
