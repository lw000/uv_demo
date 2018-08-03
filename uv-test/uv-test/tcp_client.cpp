//
//  tcp_client.cpp
//  uv-test
//
//  Created by 李伟 on 2018/7/31.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>

#include "uv.h"

#include "tcp_client.hpp"
#include "net_iobuffer.h"
#include "data_struct.hpp"

#define DEFAULT_PORT 7000

static uv_loop_t* loop;
static uv_tcp_t client;
static uv_connect_t connect_req;
static NetIOBuffer iobuffer;
static std::string flag;

static void alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
static void read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
static void write_cb(uv_write_t* req, int status);
static void connect_cb(uv_connect_t* req, int status);
static void parse_data_cb(MSG* pack, void* userdata);

void parse_data_cb(MSG* msg, void* userdata) {
    
    int main_cmd = msg->main_cmd;
    int assi_cmd = msg->assi_cmd;
    char* buf = msg->buf;
    
    reponse_a_data * reponse = reinterpret_cast<reponse_a_data*>(buf);
    printf("main_id: %d, ass_id: %d, code: %d, c: %d\n", main_cmd, assi_cmd, reponse->code, reponse->c);
}

void alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    buf->base = (char*) malloc(1024);
    buf->len = 1024;
}

void read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
    if (nread < 0) {
        if (nread != UV_EOF) {
            fprintf(stderr, "read error %s\n", uv_err_name(nread));
        }
        uv_close((uv_handle_t*)&client, NULL);
    }
    else {
        iobuffer.parse(buf->base, nread, parse_data_cb, NULL);
    }
    
    if (buf->base) {
        free(buf->base);
    }
}

void write_cb(uv_write_t* req, int status) {
    free(req);
}

void entry(void *arg) {
    int i = 1;
    while (i) {
        reqest_a_data data;
        data.a = 10;
        data.b = 20;
        iobuffer.send(100, 200, (void*)&data, sizeof(data), [](NetPackage * pack) -> int {
            uv_write_t *req = (uv_write_t*)malloc(sizeof(uv_write_t));
            uv_buf_t newbuf = uv_buf_init(pack->getBuf(), pack->getSize());
            int ret = uv_write(req, (uv_stream_t*)&client, &newbuf, 1, write_cb);
            return ret;
        });
        sleep(1);
    }
}

void connect_cb(uv_connect_t* req, int status) {
    if (0 == status)
    {
        int ret = uv_read_start(req->handle, alloc_cb, read_cb);
        printf("[%d]", ret);
        uv_thread_t tid;
        uv_thread_create(&tid, entry, NULL);
    }
    else
    {
        printf("socket error. [%d]\n", status);
    }
}

int client_run(int argc, char ** args)
{
    if (argc < 3) {
        return 0;
    }
    
    flag = args[2];
    
    loop = uv_default_loop();
    uv_tcp_init(loop, &client);

    sockaddr_in addr;
    int ret = uv_ip4_addr("127.0.0.1", DEFAULT_PORT, &addr);
    ret = uv_tcp_connect(&connect_req, &client, (const sockaddr*)&addr, connect_cb);
    
    return uv_run(loop, UV_RUN_DEFAULT);
}
