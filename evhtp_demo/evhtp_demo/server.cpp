#include "example.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <string>
#include <fstream>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/reader.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/filereadstream.h>

#include <evhtp/evhtp.h>

#include <log4z/log4z.h>

#include "data.h"
#include "bunissfunc.h"

using namespace zsummer::log4z;
using namespace rapidjson;

bool load_config() {
	FILE *f = NULL;
	do {
		f = fopen("./user_info.cfg", "r");
		fseek(f, 0, SEEK_SET);
        char buff[2048];
		Document root;
		FileReadStream inputStream(f, buff, sizeof(buff));
		root.ParseStream<0>(inputStream);
		if (root.HasParseError()) {
            fclose(f);
			LOGE("fail parse json.");
			break;
		}
        fclose(f);
		
        if (root.Empty()) {
            LOGE("json is empty.");
            break;
        }
        
        if (!root.IsArray()) {
			LOGE("json is not object array.");
			break;
		}

		for (rapidjson::SizeType i = 0; i < root.Size(); i++) {
            rapidjson::Value& val = root[i];
            
            User user;
            if (val.HasMember("uid")) {
                user.uid = val["uid"].GetString();
            }
            if (val.HasMember("name")) {
                user.name = val["name"].GetString();
            }
            if (val.HasMember("psd")) {
                user.psd = val["psd"].GetString();
            }
			g_users.insert(std::make_pair(user.uid, user));
		}
	} while (0);

	return true;
}

void __thread_init_cb(evhtp_t * htp, evthr_t * thr, void * arg) {
    char* s = (char*)(arg);
    if (strcmp(s, "ipv4") == 0) {
        LOGD("thread_init_cb ipv4");
    }
    
    if (strcmp(s, "ipv6") == 0) {
        LOGD("thread_init_cb ipv6");
    }
}

void __thread_init_exit(evhtp_t * htp, evthr_t * thr, void * arg) {
    char* s = (char*)(arg);
    if (strcmp(s, "ipv4") == 0) {
        LOGD("thread_init_exit ipv4");
    }
    
    if (strcmp(s, "ipv6") == 0) {
        LOGD("thread_init_exit ipv6");
    }
}

void __genreq_cb(evhtp_request_t * req, void * arg) {
	const char * ver = (const char *) arg;

	evbuffer_add(req->buffer_out, ver, strlen(ver));
	evhtp_send_reply(req, EVHTP_RES_OK);
}

int main_server(int port) {
	if (!load_config()) {
		return -1;
	}

	srand((unsigned)time(NULL));

	evbase_t * evbase = NULL;
	evhtp_t * htp_v4 = NULL;
	evhtp_t * htp_v6 = NULL;

	evbase = event_base_new();

	htp_v4 = evhtp_new(evbase, NULL);
	htp_v6 = evhtp_new(evbase, NULL);

	evhtp_use_threads_wexit(htp_v4, __thread_init_cb, __thread_init_exit, 2, (void*)"ipv4");
    evhtp_use_threads_wexit(htp_v6, __thread_init_cb, __thread_init_exit, 2, (void*)"ipv6");

//	evhtp_t * v1 = evhtp_new(evbase, NULL);
//	const char* vhost = "host1.com";
//	const char*path = "/host1";
//	evhtp_set_cb(v1, path, vh_testcb, (void*)vhost);
//	evhtp_add_vhost(htp, vhost, v1);

	{
		evhtp_set_gencb(htp_v4, __genreq_cb, (void *) "ipv4");
		evhtp_set_gencb(htp_v6, __genreq_cb, (void *) "ipv6");
	}

	{
		evhtp_callback_t * htpcb = NULL;
		htpcb = evhtp_set_cb(htp_v4, "/register", http_registercb, NULL);
		htpcb = evhtp_set_cb(htp_v6, "/register", http_registercb, NULL);
		assert(htpcb != NULL);

		htpcb = evhtp_set_cb(htp_v4, "/login", http_logincb, NULL);
		htpcb = evhtp_set_cb(htp_v6, "/login", http_logincb, NULL);
		assert(htpcb != NULL);

		htpcb = evhtp_set_cb(htp_v4, "/test", http_testcb, NULL);
		htpcb = evhtp_set_cb(htp_v6, "/test", http_testcb, NULL);
		assert(htpcb != NULL);

		htpcb = evhtp_set_cb(htp_v4, "/add", http_addcb, NULL);
		htpcb = evhtp_set_cb(htp_v6, "/add", http_addcb, NULL);
		assert(htpcb != NULL);
        
        htpcb = evhtp_set_cb(htp_v4, "/sub", http_subcb, NULL);
        htpcb = evhtp_set_cb(htp_v6, "/sub", http_subcb, NULL);
        assert(htpcb != NULL);

		htpcb = evhtp_set_cb(htp_v4, "/fact", http_factcb, NULL);
		htpcb = evhtp_set_cb(htp_v6, "/fact", http_factcb, NULL);
		assert(htpcb != NULL);
	}

	do {
        int r = 0;
        r = evhtp_bind_socket(htp_v6, "ipv6:::/128", port, 1024);
        if (r == 0) {
            LOGD("bind ipv6 " << "[" << port << "]" << "success");
        }
        else {
            LOGD("bind ipv6 fail");
            break;
        }

        r = evhtp_bind_socket(htp_v4, "ipv4:0.0.0.0", port, 1024);
        if (r == 0) {
            LOGD("bind ipv4 " << "[" << port << "]" << "success");
        } else {
            LOGD("bind ipv4 fail");
            break;
        }

        LOGD("running port: [" << port << "]");

        r = event_base_dispatch(evbase);
        if (r != 0) {
            LOGD("event_base_dispatch fail [" << r << "]");
            break;
        }
        
	} while (0);

	evhtp_unbind_socket(htp_v4);
	evhtp_unbind_socket(htp_v6);

	event_base_free(evbase);

	return 0;
}
