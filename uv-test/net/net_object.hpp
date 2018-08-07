//
//  net_object.hpp
//  uv-test
//
//  Created by 李伟 on 2018/8/7.
//  Copyright © 2018年 李伟. All rights reserved.
//

#ifndef net_object_hpp
#define net_object_hpp

#include <stdio.h>
#include <string>

class Object {
public:
    Object();
    ~Object();
    
public:
    void setUserData(void* data);
    void* getUserData() const;
    void setTag(int tag);
    int getTag() const;
    void setName(const std::string& name);
    std::string getName() const;
    std::string debug();
    
private:
    void* _data;
    int _tag;
    std::string _name;
};

#endif /* net_object_hpp */
