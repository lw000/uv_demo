//
//  data.hpp
//  rpclib_demo
//
//  Created by 李伟 on 2018/8/9.
//  Copyright © 2018年 李伟. All rights reserved.
//

#ifndef data_hpp
#define data_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>

typedef struct _rpc_base_request {
    
} rpc_base_request;

typedef struct _rpc_base_reponse {
    int code;
    std::string what;
} rpc_base_reponse;

#define PROPERTY(type, name)                    \
void set##name(const type& #name) {             \
    this->#name = #name;                        \
}                                               \
std::string get##name() {                       \
    return this->#name;                         \
}                                               \


typedef struct _rpc_register_result_request : public _rpc_base_request {
    std::string phone;
    std::string name;
    std::string psd;
    
public:
    std::string encode();
    
} rpc_register_result_request;

typedef struct _rpc_register_result_reponse : public _rpc_base_reponse {
    std::string uid;
    
public:
    int decode(const std::string& data);
    
} rpc_register_result_reponse;


typedef struct _rpc_login_result_request : public _rpc_base_request {
    std::string uid;
    std::string psd;
    
public:
    std::string encode();
    
} rpc_login_result_request;

typedef struct _rpc_login_result_reponse : public _rpc_base_reponse {
    std::string uid;
    std::string session;
    
public:
    int decode(const std::string& data);
    
} rpc_login_result_reponse;

#endif /* data_hpp */
