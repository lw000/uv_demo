//
//  main.cpp
//  nlohmann_json_demo
//
//  Created by 李伟 on 2018/8/15.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include <string>
#include <iostream>
#include <nlohmann/json.hpp>

int main(int argc, const char * argv[]) {
    nlohmann::json j;
    
    // add a number that is stored as double (note the implicit conversion of j to an object)
    j["pi"] = 3.141;
    
    // add a Boolean that is stored as bool
    j["happy"] = true;
    
    // add a string that is stored as std::string
    j["name"] = "Niels";
    
    // add another null object by passing nullptr
    j["nothing"] = nullptr;
    
    // add an object inside the object
    j["answer"]["everything"] = 42;
    
    // add an array that is stored as std::vector (using an initializer list)
    j["list"] = { 1, 0, 2 };
    
    // add another object (using an initializer list of pairs)
    j["object"] = { {"currency", "USD"}, {"value", 42.99} };
    
    std::string s = j.dump();
    std::cout << s << std::endl;
    
    // instead, you could also write (which looks very similar to the JSON above)
    nlohmann::json j2 = {
        {"pi", 3.141},
        {"happy", true},
        {"name", "Niels"},
        {"nothing", nullptr},
        {"answer", {{"everything", 42}}},
        {"list", {1, 0, 2}},
        {"object", {
            {"currency", "USD"},
            {"value", 42.99}
        }}
    };
    
    std::string s2 = j2.dump();
    std::cout << s2 << std::endl;
    
    {
        nlohmann::json j;
        j.push_back("foo");
        j.push_back(1);
        j.push_back(true);
        
        // also use emplace_back
        j.emplace_back(1.78);
        
        // iterate the array
        for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it) {
            std::cout << *it << '\n';
        }
        
        // range-based for
        for (auto& element : j) {
            std::cout << element << '\n';
        }
    }
    
    {
        std::vector<std::uint8_t> v = {'t', 'r', 'u', 'e'};
        nlohmann::json j = nlohmann::json::parse(v);
        std::string  s = j.dump();
        std::cout << s << std::endl;
    }
    
    return 0;
}
