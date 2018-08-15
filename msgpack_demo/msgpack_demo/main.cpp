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

struct myclass {
    myclass() : num(0), str("default") { }
    
    myclass(int num, const std::string& str) :
    num(0), str("default") { }
    
    ~myclass() { }
    
    int num;
    std::string str;
    std::vector<std::string> vs;
    
    MSGPACK_DEFINE(num, str);
    
    bool operator==(const myclass& o) const
    {
        return num == o.num && str == o.str;
    }
};

std::ostream& operator<<(std::ostream& o, const myclass& m)
{
    return o << "myclass(" << m.num << ",\"" << m.str << "\")";
}

int main(int argc, const char * argv[]) {

    {
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
    }
    
    {
        myclass m1;
        m1.num = 10000;
        m1.str = "22222222";
        m1.vs.push_back("111111111");
        m1.vs.push_back("111111111");
        m1.vs.push_back("111111111");
        m1.vs.push_back("111111111");
        
        std::cout << "m1: " << m1 << std::endl;
        
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, m1);
        
        msgpack::zone z;
        msgpack::object obj;
        msgpack::unpack_return ret = msgpack::unpack(sbuf.data(), sbuf.size(), NULL, &z, &obj);
        
        if (ret == msgpack::UNPACK_SUCCESS) {
            myclass m2;
            obj.convert(&m2);
            std::cout << "m2: " << m1 << std::endl;
        }
        
        if (m1 == obj.as<myclass>()) {
            std::cout << " ok " << std::endl;
        }
    }
    
    return 0;
}
