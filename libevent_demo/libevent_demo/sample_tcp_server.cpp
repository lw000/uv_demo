//
//  tcp_server.cpp
//  libevent_demo
//
//  Created by 李伟 on 2018/8/15.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include "sample_tcp_server.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <event.h>


static void accept_cb(int fd, short events, void* arg);
static void read_cb(int fd, short events, void* arg);

static int tcp_server_init(int port, int listen_num);

void accept_cb(int fd, short events, void* arg) {
    evutil_socket_t sockfd;
    
    struct sockaddr_in addr_in;
    socklen_t len = sizeof(addr_in);
    sockfd = ::accept(fd, (struct sockaddr*)&addr_in, &len);
    evutil_make_socket_nonblocking(sockfd);
    
    printf("accept a client %d \n", sockfd);
    
    struct event_base* base = (event_base*)arg;
    struct event* ev = event_new(base, -1, 0, NULL, NULL);
    event_assign(ev, base, sockfd, EV_READ|EV_PERSIST, read_cb, ev);
    
    event_add(ev, NULL);
}

void read_cb(int fd, short events, void* arg) {
    int error_save;
    char msg[4096];
    struct event * ev = (struct event*)(arg);
    size_t rlen = ::read(fd, msg, sizeof(msg) - 1);

    if (rlen < 0) {
        error_save = errno;
        if (error_save == EINTR) {
            
        }
        
        if (error_save == EPIPE) {
            
        }
        printf("some error happen when read\n");
        event_free(ev);
        return;
    } else if (rlen == 0) {
    
    }
    else {
        
    }
    
    msg[rlen] = '\0';
    
    char reply_msg[4096] = "i have recvieced the msg:";
    size_t wr = write(fd, reply_msg, strlen(reply_msg));
    if (wr <= 0) {
        error_save = errno;
        if (error_save == EINTR) {
            
        }
        
        if (error_save == EPIPE) {
            
        }
    }
}

int tcp_server_init(int port, int listen_num) {
    int error_save;
    
    evutil_socket_t listener;
    listener = ::socket(AF_INET, SOCK_STREAM, 0);
    if (listener == -1) {
        return -1;
    }
    
    evutil_make_listen_socket_reuseable(listener);
    
    struct sockaddr_in addr_in;
    addr_in.sin_family = AF_INET;
    addr_in.sin_addr.s_addr = 0;
    addr_in.sin_port = htons(port);
    
    if (::bind(listener, (struct sockaddr*)&addr_in, sizeof(addr_in)) < 0) {
        goto error;
    }
    
    if (::listen(listener, listen_num) < 0) {
        goto error;
    }
    
    evutil_make_socket_nonblocking(listener);
    
    return listener;
    
error:
    error_save = errno;
    evutil_closesocket(listener);
    errno = error_save;
    
    return -1;
}

int main_server_run(int argc, const char** argv) {
    
    return 0;
}
