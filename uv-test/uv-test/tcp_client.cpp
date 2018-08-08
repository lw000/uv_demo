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

static uv_loop_t* loop;
static uv_tcp_t client;
static std::string flag;
static NetIOBuffer iobuffer;

static void alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
static void read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
static void write_cb(uv_write_t* req, int status);
static void connect_cb(uv_connect_t* req, int status);
static void parse_cb(MSG* pack, void* userdata);

typedef struct {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;

static void free_write_req(uv_write_t *req) {
    write_req_t *wr = (write_req_t*) req;
    free(wr->buf.base);
    free(wr);
}

void parse_cb(MSG* msg, void* userdata) {
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
        iobuffer.parse(buf->base, nread, parse_cb, NULL);
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
        iobuffer.send(100, 200, (void*)&data, sizeof(data), [](NetPacket * pkt) -> int {
            uv_write_t *req = (uv_write_t*)malloc(sizeof(uv_write_t));
            uv_buf_t newbuf = uv_buf_init(pkt->Buffer(), pkt->BufferSize());
            int r = uv_write(req, (uv_stream_t*)&client, &newbuf, 1, write_cb);
            return r;
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
    
    loop = uv_loop_new();
    uv_tcp_init(loop, &client);

    uv_connect_t* connect_req = (uv_connect_t*)::malloc(sizeof(uv_connect_t));
    sockaddr_in addr;
    int r = uv_ip4_addr("127.0.0.1", 7000, &addr);
    r = uv_tcp_connect(connect_req, &client, (const sockaddr*)&addr, connect_cb);
    if (r != 0) {
    
    }
    
    r = uv_run(loop, UV_RUN_DEFAULT);
    if (r != 0) {
        
    }
    
    uv_loop_close(loop);
    
    free(connect_req);
    
    free(loop);
    
    return r;
}
