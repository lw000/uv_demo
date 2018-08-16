//
//  epoll_server.cpp
//  
//
//  Created by 李伟 on 2018/8/16.
//

#include "epoll_server.hpp"

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

static void setnonblocking(int sock) {
    int opts;
    opts = fcntl(sock, F_GETFL);
    if (opts < 0) {
        perror("fcntl (sock, GETFL)");
        return;
    }
    opts = opts | O_NONBLOCK;
    if (fcntl(sock, F_SETFL, opts) < 0) {
        perror("fcntl (sock, SETFL, opts)");
        return;
    }
}
