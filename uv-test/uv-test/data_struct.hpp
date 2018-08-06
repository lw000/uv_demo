//
//  data_struct.hpp
//  uv-test
//
//  Created by 李伟 on 2018/8/1.
//  Copyright © 2018年 李伟. All rights reserved.
//

#ifndef data_struct_hpp
#define data_struct_hpp

enum class SysCmd{
    login = 1,          //登陆系统
};

enum class BusCmd{
    login = 1,          //登陆系统
};

#pragma pack(1)

struct reqest_a_data {
    int a;
    int b;
};

struct reponse_a_data {
    int code;
    int c;
};

struct reqest_add_data {
    int a;
    int b;
};

struct reponse_add_data {
    int code;
    int c;
};

struct reqest_login_data {
    char uname[64];
    char upsd[64];
};

struct reponse_login_data {
    int code;
    char what[128];
};

#pragma pack()

#endif /* data_struct_hpp */
