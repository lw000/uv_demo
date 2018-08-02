//
//  main.cpp
//  wsclient
//
//  Created by 李伟 on 2018/7/26.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <libwebsockets.h>

static int destroy_flag = 0;
static int connection_flag = 0;
static int writeable_flag = 0;

static void INT_HANDLER(int signo) {
    destroy_flag = 0;
}

struct session_data {
    int fd;
};

struct pthread_routine_tool {
    struct lws_context *context;
    struct lws *wsi;
};

static const struct lws_extension exts[] = {
    {
        "permessage-deflate",
        lws_extension_callback_pm_deflate,
        "permessage-deflate"
    },
    { NULL, NULL, NULL /* terminator */ }
};

static int websocket_write_back(struct lws *wsi_in, char *str, int str_size_in)
{
    if (str == NULL || wsi_in == NULL)
        return -1;
    
    int n;
    int len;
    unsigned char *out = NULL;
    
    if (str_size_in < 1)
        len = strlen(str);
    else
        len = str_size_in;
    
    out = (unsigned char *)malloc(sizeof(unsigned char)*(LWS_SEND_BUFFER_PRE_PADDING + len + LWS_SEND_BUFFER_POST_PADDING));
    //* setup the buffer*/
    memcpy (out + LWS_SEND_BUFFER_PRE_PADDING, str, len );
    //* write out*/
    n = lws_write(wsi_in, out + LWS_SEND_BUFFER_PRE_PADDING, len, LWS_WRITE_TEXT);
    
    lwsl_info("[websocket_write_back] %s\n", str);
    //* free the buffer*/
    free(out);
    
    return n;
}


static int ws_service_callback(
                               struct lws *wsi,
                               enum lws_callback_reasons reason, void *user,
                               void *in, size_t len)
{
    
    switch (reason) {
            
        case LWS_CALLBACK_CLIENT_ESTABLISHED:
            lwsl_info("[Main Service] Connect with server success.\n");
            connection_flag = 1;
            break;
            
        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            lwsl_info("[Main Service] Connect with server error.\n");
            destroy_flag = 1;
            connection_flag = 0;
            break;
            
        case LWS_CALLBACK_CLOSED:
            lwsl_info("[Main Service] LWS_CALLBACK_CLOSED\n");
            destroy_flag = 1;
            connection_flag = 0;
            break;
            
        case LWS_CALLBACK_CLIENT_RECEIVE:
            lwsl_info("[Main Service] Client recvived:%s\n", (char *)in);
            
            if (writeable_flag)
                destroy_flag = 1;
            
            break;
        case LWS_CALLBACK_CLIENT_WRITEABLE :
            lwsl_info("[Main Service] On writeable is called. send byebye message\n");
            websocket_write_back(wsi, "Byebye! See you later", -1);
            writeable_flag = 1;
            break;
            
        default:
            break;
    }
    
    return 0;
}

static void *pthread_routine(void *tool_in)
{
    struct pthread_routine_tool *tool = (struct pthread_routine_tool*)tool_in;
    
    lwsl_info("[pthread_routine] Good day. This is pthread_routine.\n");
    
    //* waiting for connection with server done.*/
    while(!connection_flag)
        usleep(1000*20);
    
    //*Send greeting to server*/
    lwsl_info("[pthread_routine] Server is ready. send a greeting message to server.\n");
    websocket_write_back(tool->wsi, "Good day", -1);
    
    lwsl_info("[pthread_routine] sleep 2 seconds then call onWritable\n");
    sleep(1);
    lwsl_info("------------------------------------------------------\n");
    sleep(1);
    //printf(KBRN"[pthread_routine] sleep 2 seconds then call onWritable\n"RESET);
    
    //*involked wriable*/
    lwsl_info("[pthread_routine] call on writable.\n");
    lws_callback_on_writable(tool->wsi);
    
    return 0;
}

int main(int argc, const char * argv[]) {
    struct sigaction act;
    act.sa_handler = INT_HANDLER;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaction( SIGINT, &act, 0);
    
    
    struct lws_context *context = NULL;
    struct lws_context_creation_info info;
    struct lws_client_connect_info i;
    struct lws *wsi = NULL;
    struct lws_protocols protocol;
    
    memset(&info, 0, sizeof info);
    info.port = CONTEXT_PORT_NO_LISTEN;
    info.iface = NULL;
    info.protocols = &protocol;
    info.ssl_cert_filepath = NULL;
    info.ssl_private_key_filepath = NULL;
    //info.extensions = libwebsocket_get_internal_extensions();
    info.extensions = exts;
    info.gid = -1;
    info.uid = -1;
    info.options = 0;
    
    protocol.name  = "my-echo-protocol";
    protocol.callback = &ws_service_callback;
    protocol.per_session_data_size = sizeof(struct session_data);
    protocol.rx_buffer_size = 0;
    protocol.id = 0;
    protocol.user = NULL;
    
    context = lws_create_context(&info);
    lwsl_info("[Main] context created.\n");
    
    if (context == NULL) {
        lwsl_info("[Main] context is NULL.\n");
        return -1;
    }
    
    memset(&i, 0, sizeof(i));
    i.context = context;
    i.pwsi = &wsi;
    //"localhost", 5000, 0, "/", "localhost:5000", NULL, protocol.name, -1
    i.host = "localhost";
    i.port = 5000;
    i.path = "/";
    i.protocol = protocol.name;
    
    wsi = lws_client_connect_via_info(&i);
    if (wsi == NULL) {
        lwsl_info("[Main] context is NULL.\n");
        return -1;
    }
    
    struct pthread_routine_tool tool;
    tool.wsi = wsi;
    tool.context = context;
    
    pthread_t pid;
    pthread_create(&pid, NULL, pthread_routine, &tool);
    pthread_detach(pid);
    
    while (!destroy_flag) {
        lws_service(context, 50);
    }
    
    lws_context_destroy(context);
    

    return 0;
}
