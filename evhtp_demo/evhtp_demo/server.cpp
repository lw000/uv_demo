#include "example.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <string>
#include <fstream>

#include <evhtp/evhtp.h>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/reader.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/filereadstream.h>

#include <log4z/log4z.h>

#include "data.h"
#include "bunissfunc.h"

using namespace zsummer::log4z;
using namespace rapidjson;


bool load_config() {
	FILE *f = NULL;
	do {
		f = fopen("./config.cfg", "r");
		fseek(f, 0, SEEK_SET);
        char buff[2048];
		Document root;
		FileReadStream inputStream(f, buff, sizeof(buff));
		root.ParseStream<0>(inputStream);
		if (root.HasParseError()) {
			LOGE("fail parse json.");
			break;
		}

		if (!root.IsArray()) {
			LOGE("json is not object array.");
			break;
		}

		if (root.Empty()) {
			LOGE("json is empty.");
			break;
		}

		for (rapidjson::SizeType i = 0; i < root.Size(); i++) {
			User user;
			user.uid = root[i]["uid"].GetString();
			user.uname = root[i]["uname"].GetString();
			user.psd = root[i]["psd"].GetString();
			g_users.insert(std::make_pair(user.uid, user));
		}
	} while (0);

	fclose(f);

	return true;
}

void _thread_init_cb(evhtp_t * htp, evthr_t * thr, void * arg) {
	LOGD("thread_init_cb");
}

void _thread_init_exit(evhtp_t * htp, evthr_t * thr, void * arg) {
	LOGD("thread_init_exit");
}

void _req_cb(evhtp_request_t * req, void * arg) {
	const char * ver = (const char *) arg;

	evbuffer_add(req->buffer_out, ver, strlen(ver));
	evhtp_send_reply(req, EVHTP_RES_OK);
}

int main_server(int port) {
	if (!load_config()) {
		return -1;
	}

	srand((unsigned) time(NULL));

	evbase_t * evbase = NULL;
	evhtp_t * htp_v4 = NULL;
	evhtp_t * htp_v6 = NULL;

	evbase = event_base_new();

	htp_v4 = evhtp_new(evbase, NULL);
	htp_v6 = evhtp_new(evbase, NULL);

	evhtp_use_threads_wexit(htp_v4, _thread_init_cb, _thread_init_exit, 8,
	NULL);

//	evhtp_t * v1 = evhtp_new(evbase, NULL);
//	const char* vhost = "host1.com";
//	const char*path = "/host1";
//	evhtp_set_cb(v1, path, vh_testcb, (void*)vhost);
//	evhtp_add_vhost(htp, vhost, v1);

	{
		evhtp_set_gencb(htp_v6, _req_cb, (void *) "ipv6");
		evhtp_set_gencb(htp_v4, _req_cb, (void *) "ipv4");
	}

	{
		evhtp_callback_t * cb_1 = NULL;
		evhtp_callback_t * cb_2 = NULL;
		evhtp_callback_t * cb_3 = NULL;
		evhtp_callback_t * cb_4 = NULL;
		evhtp_callback_t * cb_5 = NULL;

		cb_1 = evhtp_set_cb(htp_v4, "/register", registercb, NULL);
		cb_1 = evhtp_set_cb(htp_v6, "/register", registercb, NULL);
		assert(cb_1 != NULL);

		cb_2 = evhtp_set_cb(htp_v4, "/login", logincb, NULL);
		cb_2 = evhtp_set_cb(htp_v6, "/login", logincb, NULL);
		assert(cb_2 != NULL);

		cb_3 = evhtp_set_cb(htp_v4, "/test", testcb, NULL);
		cb_3 = evhtp_set_cb(htp_v6, "/test", testcb, NULL);
		assert(cb_3 != NULL);

		cb_4 = evhtp_set_cb(htp_v4, "/add", addcb, NULL);
		cb_4 = evhtp_set_cb(htp_v6, "/add", addcb, NULL);
		assert(cb_4 != NULL);

		cb_5 = evhtp_set_cb(htp_v4, "/fact", factcb, NULL);
		cb_5 = evhtp_set_cb(htp_v6, "/fact", factcb, NULL);
		assert(cb_5 != NULL);
	}

	do {
		{
			int r = evhtp_bind_socket(htp_v4, "ipv6:::/128", port, 1024);
			if (r != 0) {
				LOGD("bind ipv6 fail");
				break;
			}
		}

		{
			int r = evhtp_bind_socket(htp_v4, "ipv4:0.0.0.0", port, 1024);
			if (r != 0) {
				LOGD("bind ipv4 fail");
				break;
			}
		}

		//	syncRedis.connect("192.168.204.128", 6379);

		LOGD("running [port : " << port << "]");

		int r = event_base_dispatch(evbase);
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
