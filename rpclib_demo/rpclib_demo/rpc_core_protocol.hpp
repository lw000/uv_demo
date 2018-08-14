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

typedef struct __rpc_base_request {
    
} rpc_base_request;

typedef struct __rpc_base_reponse {
    int code;
    std::string what;
} rpc_base_reponse;

#define PROPERTY(type, name)                    \
private:                                        \
type __##name;                                  \
public:                                         \
void set_##name(const type& name) {             \
    this->__##name = name;                      \
}                                               \
type get_##name() {                             \
return this->__##name;                          \
}                                               \

typedef struct __rpc_register_request : public __rpc_base_request {
    std::string phone;
    std::string name;
    std::string psd;
    
    bool operator ==(const __rpc_register_request& o) const
    {
        return phone.compare(o.phone) == 0 && name.compare(o.name) == 0 && psd.compare(o.psd) == 0;
    }
    
public:
    std::string encode();
    
} rpc_register_request;

typedef struct __rpc_register_reponse : public __rpc_base_reponse {
    std::string uid;
    
    bool operator==(const __rpc_register_reponse& o) const
    {
        return uid.compare(o.uid) == 0;
    }
    
public:
    int decode(const std::string& data);
    
} rpc_register_reponse;

typedef struct __rpc_login_request : public __rpc_base_request {
    PROPERTY(std::string, uid)
    PROPERTY(std::string, psd)
    
public:
    std::string encode();
    
} rpc_login_request;

typedef struct __rpc_login_reponse : public __rpc_base_reponse {
    PROPERTY(std::string, uid)
    PROPERTY(std::string, session)
public:
    int decode(const std::string& data);
    
} rpc_login_reponse;

#endif /* data_hpp */
