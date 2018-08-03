//
//  main.cpp
//  uv-test
//
//  Created by 李伟 on 2018/7/31.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include <string.h>
#include <unistd.h>

#include "tcp_server.hpp"
#include "tcp_client.hpp"

#include "TCPClient.h"
#include "TCPServer.h"

#include "uv.h"

#include "data_struct.hpp"



class CliServer : public lw::TCPClient {
public:
    CliServer() {
        
    }
    
    ~CliServer() {
        
    }
    
public:
    class Delegate : public TCPClient::Delegate {
    public:
        Delegate() {
            
        }
    
    public:
        void onMessage(NetPackage* msg) override {
            
        }
        
        void onStatus(int status) override {
            
        }
    };
    
public:
    virtual void onMessage(NetPackage* message) override {
        if (message->getHead()->msg_main_cmd == 1000 && message->getHead()->msg_assi_cmd == 2000) {
            reponse_add_data* reponse = (reponse_add_data*)message;
            printf("code: %d, c: %d\n", reponse->code, reponse->c);
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

            sleep(1);
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
    virtual void onMessage(uv_stream_t* client, NetPackage* message) override {
        
        if (message->getHead()->msg_main_cmd == 1000 && message->getHead()->msg_assi_cmd == 2000) {
            reqest_add_data* request = (reqest_add_data*)(message->getBuf());
            printf("a: %d, b: %d\n", request->a, request->b);
            
            {
                reponse_add_data reponse;
                reponse.c = request->a + request->b;
                reponse.code = 0;
                this->sendData((uv_tcp_s*)client, 1000, 2000, &reponse, sizeof(reponse));
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

int main(int argc, char** args)
{
    if (strcmp(args[1], "-s") == 0) {
        server_run(argc, args);
    }
    else if(strcmp(args[1], "-c") == 0) {
        client_run(argc, args);
    } else if (strcmp(args[1], "-ss") == 0) {
        SrvServer srv;
        srv.syncStart("0.0.0.0", 9090);
    }
    else if(strcmp(args[1], "-cc") == 0) {
        CliServer srv;
        srv.syncStart("127.0.0.1", 9090);
    }
    else {
        printf("-s or -c");
    }
    
    return 0;
}
