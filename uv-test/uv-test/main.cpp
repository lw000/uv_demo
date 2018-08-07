//
//  main.cpp
//  uv-test
//
//  Created by 李伟 on 2018/7/31.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include <string.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <map>

#include "uv.h"

#include "tcp_server.hpp"
#include "tcp_client.hpp"
#include "timer_server.hpp"
#include "uv_querying_dns.hpp"
#include "work_demo.hpp"
#include "http_server.hpp"
#include "net_request.hpp"

#include "TCPClient.h"
#include "TCPServer.h"

#include "data_struct.hpp"


class CliServer : public lw::TCPClient {
public:
    CliServer() {
        
    }
    
    ~CliServer() {
        
    }
    
public:
    virtual void onMessage(MSG* msg) override {
        int main_cmd = msg->main_cmd;
        int assi_cmd = msg->assi_cmd;
        char* buf = msg->buf;
        
        if (main_cmd == 1000 && assi_cmd == 2000) {
            reponse_add_data* reponse = reinterpret_cast<reponse_add_data*>(buf);
            printf("recv code: %d, c: %d\n", reponse->code, reponse->c);
        }
    }
    
    void onStatus(int status) override {
        if (status == 0) {
            sendTest();
        }
    }
    
public:
    void sendTest() {
        uv_thread_t tid;
        uv_thread_create(&tid, entry, this);
    }
    
private:
    static void entry(void* args) {
        while (1) {
            CliServer* cli = (CliServer*)args;
            
            {
                reqest_add_data request;
                request.a = 1000;
                request.b = 2000;
                cli->sendData(1000, 2000, &request, sizeof(request));
            }

            sleep(0.5);
        }
    }
};

class SrvServer : public lw::TCPServer {

public:
    SrvServer() {
        
    }
    
    ~SrvServer() {
        
    }
    
public:
    virtual void onMessage(uv_stream_t* cli, MSG* msg) override {
        int main_cmd = msg->main_cmd;
        int assi_cmd = msg->assi_cmd;
        char* buf = msg->buf;
        
        if (main_cmd == 1000 && assi_cmd == 2000) {
            reqest_add_data* request = reinterpret_cast<reqest_add_data*>(buf);
            {
                printf("recv a: %d, b: %d\n", request->a, request->b);
            }
            
            {
                reponse_add_data reponse;
                reponse.c = request->a + request->b;
                reponse.code = 0;
                this->sendData((uv_tcp_t*)cli, 1000, 2000, &reponse, sizeof(reponse));
            }
        }
    }
    
    void onStatus(int status) override {
        if (status == 0) {
            
        }
    }

private:
    static void entry(void* args) {
        
    }
};

struct CONFFIG {
    std::string c;
    std::string v;
    
public:
    CONFFIG(const std::string& c, const std::string& v) {
        this->c = c;
        this->v = v;
    }
    
    bool operator < (const CONFFIG& conf) const {
        return (this->c.compare(conf.v) < 0) || (this->v.compare(conf.v) < 0);
    }
};

int main(int argc, char** args)
{
    std::vector<CONFFIG> config = {
        {"-s", "server_run"},
        {"-c", "client_run"},
        {"-ss", "SrvServer"},
        {"-cc", "server_run"},
        {"-t", "timer_run"}
    };
    
    std::map<std::string, std::string> config_map;
    config_map["-s"] = "server_run";
    config_map["-c"] = "client_run";
    config_map["-ss"] = "SrvServer";
    config_map["-cc"] = "server_run";
    config_map["-t"] = "timer_run";
    
    std::map<CONFFIG, std::string> map_config;
    map_config[CONFFIG("1", "1")] = "1111111111";
    map_config[CONFFIG("2", "2")] = "2222222222";
    map_config[CONFFIG("3", "3")] = "3333333333";
    map_config[CONFFIG("4", "4")] = "4444444444";
    map_config[CONFFIG("5", "5")] = "5555555555";
    
    uv_cpu_info_t *info;
    int cpu_count;
    uv_cpu_info(&info, &cpu_count);
    uv_free_cpu_info(info, cpu_count);
    
    Request req;
    req.setBody("11111111111111111111111");
    
    if (strcmp(args[1], "-s") == 0) {
        server_run(argc, args);
    }
    else if(strcmp(args[1], "-c") == 0) {
        client_run(argc, args);
    } else if (strcmp(args[1], "-ss") == 0) {
        SrvServer srv;
        srv.run("0.0.0.0", 9090);
    }
    else if(strcmp(args[1], "-cc") == 0) {
        CliServer srv;
        srv.run("127.0.0.1", 9090);
    }
    else if(strcmp(args[1], "-t") == 0) {
        timer_run(argc, args);
    }
    else if(strcmp(args[1], "-q") == 0) {
        querting_dns_run(argc, args);
    }
    else if(strcmp(args[1], "-w") == 0) {
        work_demo_run(argc, args);
    }
    else if(strcmp(args[1], "-h") == 0) {
        http_server_run(argc, args);
    }
    else {
        printf("-s or -c");
    }
    
    return 0;
}
