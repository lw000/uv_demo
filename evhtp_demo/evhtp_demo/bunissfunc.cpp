/*
 * bunissfunc.cpp
 *
 *  Created on: May 16, 2018
 *      Author: root
 */

#include "bunissfunc.h"
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <mutex>
#include <algorithm>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/reader.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "data.h"

#include <log4z/log4z.h>
using namespace zsummer::log4z;

////////////////////////////////////////////////////////////////////////////////////////////////////

long long guid = 10000;
std::mutex user_table_lock;
std::mutex user_lock;

class Buniss {
public:
	static int add(int a, int b) {
		return a + b;
	}

	static int sub(int a, int b) {
		return a - b;
	}

	static int fact(int v) {
		if (v == 0) {
			return 1;
		}
		return v * fact(v - 1);
	}
};

static int dump_kvs(evhtp_kv_t * kvobj, void * arg) {
	LOGD(kvobj->key << " : " << kvobj->val);
	return 0;
}

static int reply_json(evhtp_request_t * req, int code, const std::string& what, const std::map<std::string, std::string>& dataMap) {
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    doc.AddMember("code", code, allocator);
    doc.AddMember("what", rapidjson::Value(what.c_str(), (unsigned)what.size()), allocator);
    rapidjson::Value data;
    data.SetObject();
    {
        for (auto &m : dataMap) {
            rapidjson::Value k;
            k.SetString(m.first.c_str(), (unsigned)m.first.size());
            rapidjson::Value v;
            v.SetString(m.second.c_str(), (unsigned)m.second.size());
            data.AddMember(k, v, allocator);
        }
        doc.AddMember("data", data, allocator);
    }
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    std::string result = buffer.GetString();
    
    evbuffer_add_printf(req->buffer_out, "%s", result.c_str());
    evhtp_send_reply(req, EVHTP_RES_OK);
    
    LOGD(result);
    
    return 0;
}

int registerAllfunction(evhtp_request_t * req, void * args) {

	return 0;
}

void http_registercb(evhtp_request_t * req, void * args) {
	if (evhtp_request_get_method(req) != htp_method_POST) {
//        evbuffer_add_printf(req->buffer_out,
//                            "{\"error\":\"not support, please use post method\"}");
//        evhtp_send_reply(req, EVHTP_RES_OK);
        
        reply_json(req, 0, "", {{"error", "not support, please use post method"}});
		return;
	}

	char* raw_query = (char*) req->uri->query_raw;
	evhtp_query_t * query;
	query = evhtp_parse_query(raw_query, strlen(raw_query));

	{
		evhtp_kvs_for_each(query, dump_kvs, NULL);
	}

//	{
//		evhtp_headers_for_each(req->headers_in, dump_kvs, NULL);
//	}

	std::string username = evhtp_kv_find(query, "username");
	std::string pasword = evhtp_kv_find(query, "password");

	// 查找用户是否已经注册
	bool exist = usermgr.existWithName(username);
	if (exist) {
        reply_json(req, 0, "user exist", {{"uid", "user exist"}});
		return;
	}

	// 注册用户
	char uid_buff[24];
	User user;
	long long uid = 0;
	{
		std::lock_guard<std::mutex> lock(user_table_lock);
		uid = guid++;
		sprintf(uid_buff, "%lld", uid);

		user.uid = uid_buff;
		user.name = username;
		user.psd = pasword;
		usermgr.add(user);
	}

	// 返回结果
    reply_json(req, 0, "success", {{"uid", "1000000"}});
}

void http_logincb(evhtp_request_t * req, void * args) {
	if (evhtp_request_get_method(req) != htp_method_POST) {
        reply_json(req, 0, "", {{"error", "not support, please use post method"}});
		return;
	}

	char* raw_query = (char*) req->uri->query_raw;
	evhtp_query_t * query;
	query = evhtp_parse_query(raw_query, strlen(raw_query));

	evhtp_kvs_for_each(query, dump_kvs, NULL);

	std::string userid = evhtp_kv_find(query, "userid");
	std::string username = evhtp_kv_find(query, "username");
	std::string password = evhtp_kv_find(query, "password");

	auto v = g_users.find(userid);
	if (v == g_users.end()) {
        reply_json(req, -1, "user does not exist", {{"session", ""}});
		return;
	}

	if (v->second.name.compare(username) != 0) {
        reply_json(req, -1, "username is error", {{"session", ""}});
		return;
	}

	if (v->second.psd.compare(password) != 0) {
        reply_json(req, -1, "psd is error", {{"session", ""}});
		return;
	}

    reply_json(req, 0, "login success", {{"session", "1111111111"}});
	return;
}

void http_testcb(evhtp_request_t * req, void * args) {
//	const char* data = "{\"d\":\"ok\"}";
//	evbuffer_add_reference(req->buffer_out, data, strlen(data), NULL, NULL);
//	evhtp_send_reply(req, EVHTP_RES_OK);

	struct evbuffer * b = evbuffer_new();
	evhtp_send_reply_start(req, EVHTP_RES_OK);

	evbuffer_add(b, "foo", 3);
	evhtp_send_reply_body(req, b);
//	evbuffer_add(b, "bar", 3);
//	evhtp_send_reply_body(req, b);
	evhtp_send_reply_end(req);
	evbuffer_free(b);
}

void http_vh_testcb(evhtp_request_t * req, void * args) {
	const char* data = "{\"d\":\"ok\"}";
	evbuffer_add_reference(req->buffer_out, data, strlen(data), NULL, NULL);
	evhtp_send_reply(req, EVHTP_RES_OK);
}

void http_factcb(evhtp_request_t * req, void * args) {
	evhtp_query_t * query;
	char* raw_query = (char*) req->uri->query_raw;
	query = evhtp_parse_query(raw_query, strlen(raw_query));

	evhtp_kvs_for_each(query, dump_kvs, NULL);

	const char* v = evhtp_kv_find(query, "v");

    int iv = atoi(v);
    int result = Buniss::fact(iv);
    reply_json(req, 0, "success", {{"result", std::to_string(result)}});
}

void http_addcb(evhtp_request_t * req, void * args) {
	evhtp_query_t * query;
//	char* raw_query = (char*) req->uri->query_raw;
//	query = evhtp_parse_query(raw_query, strlen(raw_query));
	query = req->uri->query;

//	{
//		char buf[1024] = { 0 };
//		evbuffer_copyout(req->buffer_in, buf, evhtp_request_content_len(req));
//		buf[evhtp_request_content_len(req)] = '\0';
//		LOGD(buf);
//	}

	evhtp_kvs_for_each(query, dump_kvs, NULL);

	const char* a = evhtp_kv_find(query, "a");
	const char* b = evhtp_kv_find(query, "b");
	if (a == NULL || b == NULL) {
        
        reply_json(req, -1, "parameter deletion", {});
		return;
	}

	int ia = atoi(a);
	int ib = atoi(b);
	int result = Buniss::add(ia, ib);

    reply_json(req, 0, "success", {{"result", std::to_string(result)}});
}

void http_subcb(evhtp_request_t * req, void * args) {
	evhtp_query_t * query;
	char* raw_query = (char*) req->uri->query_raw;
	query = evhtp_parse_query(raw_query, strlen(raw_query));

	{
		int key_idx = 0;
		evhtp_kvs_for_each(query, dump_kvs, &key_idx);
	}

	const char* a = evhtp_kv_find(query, "a");
	const char* b = evhtp_kv_find(query, "b");
	if (a == NULL || b == NULL) {
        reply_json(req, -1, "parameter deletion", {});
		return;
	}

	int ia = atoi(a);
	int ib = atoi(b);
	int result = Buniss::sub(ia, ib);
    reply_json(req, 0, "success", {{"result", std::to_string(result)}});
}
