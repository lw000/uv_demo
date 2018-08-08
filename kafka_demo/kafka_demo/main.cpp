//
//  main.cpp
//  kafka_demo
//
//  Created by 李伟 on 2018/8/8.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include <iostream>
#include <string>
#include <librdkafka/rdkafka.h>
#include <librdkafka/rdkafkacpp.h>

class MyHashPartitioner_cb : public RdKafka::PartitionerCb {
public:
    int32_t partitioner_cb(const RdKafka::Topic *topic, const std::string *key, int32_t partition_cnt, void *msg_opaque) {
        std::cout << "partition_cnt" << partition_cnt << std::endl;
        return djb_hash(key->c_str(), key->size()) % partition_cnt;
    }
    
private:
    static inline unsigned int djb_hash(const char *str, size_t len) {
        unsigned int hash = 5381;
        for (size_t i = 0; i < len; i++) {
            hash = ((hash << 5) + hash) + str[i];
        }
        std::cout << "hash1=" << hash << std::endl;
        
        return hash;
    }
};

void TestProducer() {
    
}

int main(int argc, const char * argv[]) {
    std::string brokers = "localhost";
    std::string tpoic_str = "helloworld_kugou_test";
    std::string errstr;
    MyHashPartitioner_cb hash_partitioner;
    RdKafka::Conf* conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    RdKafka::Conf* tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);
    
    if (tconf->set("partitioner_cb", &hash_partitioner, errstr)) {
        std::cerr << errstr << std::endl;
        exit(1);
    }
    
    
    return 0;
}
