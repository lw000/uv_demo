/*
 * bunissfunc.cpp
 *
 *  Created on: May 16, 2018
 *      Author: root
 */

#include "bunissfunc.h"

#include "data.h"

#include <string>
#include <unordered_map>
#include <mutex>
#include <algorithm>

#include <log4z/log4z.h>
using namespace zsummer::log4z;

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/reader.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

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

int registerAllfunction(evhtp_request_t * req, void * args) {

	return 0;
}

void http_registercb(evhtp_request_t * req, void * args) {
	if (evhtp_request_get_method(req) != htp_method_POST) {
		evbuffer_add_printf(req->buffer_out,
                            "{\"error\":\"not support, please use post method\"}");
		evhtp_send_reply(req, EVHTP_RES_OK);
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
		rapidjson::Document doc;
		doc.SetObject();
		rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
		doc.AddMember("code", 0, allocator);
		doc.AddMember("msg", "user exist", allocator);
		{
			rapidjson::Value data;
			data.SetObject();
			data.AddMember("uid", "-1", allocator);
			doc.AddMember("data", data, allocator);
		}
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		doc.Accept(writer);
		std::string result = buffer.GetString();
        
		evbuffer_add_printf(req->buffer_out, "%s", result.c_str());
		evhtp_send_reply(req, EVHTP_RES_OK);
        
		LOGD(result);

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

	// 写缓存
	{
        
	}

	// 返回结果
	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember("code", 0, allocator);
	doc.AddMember("msg", "user exist", allocator);
	{
		rapidjson::Value data;
		data.SetObject();
		data.AddMember("uid", "-1", allocator);
		doc.AddMember("data", data, allocator);
	}
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string result = buffer.GetString();
    
	evbuffer_add_printf(req->buffer_out, "%s", result.c_str());
	evhtp_send_reply(req, EVHTP_RES_OK);

	LOGD(result);
}

void http_logincb(evhtp_request_t * req, void * args) {
	if (evhtp_request_get_method(req) != htp_method_POST) {
		evbuffer_add_printf(req->buffer_out,
				"not support, please use post method");
		evhtp_send_reply(req, EVHTP_RES_OK);
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
		rapidjson::Document doc;
		doc.SetObject();
		rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
		doc.AddMember("code", -1, allocator);
		doc.AddMember("msg", "user does not exist", allocator);
		{
			rapidjson::Value data;
			data.SetObject();
			data.AddMember("session", "", allocator);
			doc.AddMember("data", data, allocator);
		}
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		doc.Accept(writer);

		std::string result = buffer.GetString();
		LOGD(result);

		evbuffer_add_printf(req->buffer_out, "%s", result.c_str());
		evhtp_send_reply(req, EVHTP_RES_OK);

		return;
	}

	if (v->second.name.compare(username) != 0) {
		rapidjson::Document doc;
		doc.SetObject();
		rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
		doc.AddMember("code", -1, allocator);
		doc.AddMember("msg", "username is error", allocator);
		{
			rapidjson::Value data;
			data.SetObject();
			data.AddMember("session", "", allocator);
			doc.AddMember("data", data, allocator);
		}
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		doc.Accept(writer);

		std::string result = buffer.GetString();

		LOGD(result);

		evbuffer_add_printf(req->buffer_out, "%s", result.c_str());
		evhtp_send_reply(req, EVHTP_RES_OK);

		return;
	}

	if (v->second.psd.compare(password) != 0) {
		rapidjson::Document doc;
		doc.SetObject();
		rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
		doc.AddMember("code", -1, allocator);
		doc.AddMember("msg", "psd is error", allocator);
		{
			rapidjson::Value data;
			data.SetObject();
			data.AddMember("session", "", allocator);
			doc.AddMember("data", data, allocator);
		}
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		doc.Accept(writer);

		std::string result = buffer.GetString();

		LOGD(result);

		evbuffer_add_printf(req->buffer_out, "%s", result.c_str());
		evhtp_send_reply(req, EVHTP_RES_OK);

		return;
	}

	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember("code", 0, allocator);
	doc.AddMember("msg", "login success", allocator);
	{
		rapidjson::Value data;
		data.SetObject();
		data.AddMember("session", "1111111111", allocator);
		doc.AddMember("data", data, allocator);
	}
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	std::string result = buffer.GetString();

	LOGD(result);

	evbuffer_add_printf(req->buffer_out, "%s", result.c_str());
	evhtp_send_reply(req, EVHTP_RES_OK);

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

	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember("code", 0, allocator);
	doc.AddMember("msg", "success", allocator);
	{
		rapidjson::Value data;
		data.SetObject();
		data.AddMember("result", result, allocator);
		doc.AddMember("data", data, allocator);
	}
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string msg = buffer.GetString();
	evbuffer_add_printf(req->buffer_out, "%s", msg.c_str());
	evhtp_send_reply(req, EVHTP_RES_OK);

	LOGD(msg);
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
		rapidjson::Document doc;
		doc.SetObject();
		rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
		doc.AddMember("code", -1, allocator);
		doc.AddMember("msg", "parameter deletion", allocator);
		{
			rapidjson::Value data;
			data.SetObject();
			doc.AddMember("data", data, allocator);
		}
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		doc.Accept(writer);
		std::string msg = buffer.GetString();
		evbuffer_add_printf(req->buffer_out, "%s", msg.c_str());
		evhtp_send_reply(req, EVHTP_RES_OK);
		LOGD(msg);
		return;
	}

	int ia = atoi(a);
	int ib = atoi(b);
	int result = Buniss::add(ia, ib);

	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember("code", 0, allocator);
	doc.AddMember("msg", "success", allocator);
	{
		rapidjson::Value data;
		data.SetObject();
//		{
//			rapidjson::Value args;
//			args.SetArray();
//			{
//				rapidjson::Value vk;
//				vk.SetObject();
//				vk.AddMember("a", ia, allocator);
//				args.PushBack(vk, allocator);
//			}
//			{
//				rapidjson::Value vk;
//				vk.SetObject();
//				vk.AddMember("b", ib, allocator);
//				args.PushBack(vk, allocator);
//			}
////			args.PushBack(ia, allocator);
////			args.PushBack(ib, allocator);
//			data.AddMember("args", args, allocator);
//		}
		data.AddMember("result", result, allocator);
		doc.AddMember("data", data, allocator);
	}
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string msg = buffer.GetString();
	evbuffer_add_printf(req->buffer_out, "%s", msg.c_str());
	evhtp_send_reply(req, EVHTP_RES_OK);

	LOGD(msg);
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
		rapidjson::Document doc;
		doc.SetObject();
		rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
		doc.AddMember("code", -1, allocator);
		doc.AddMember("msg", "parameter deletion", allocator);
		{
			rapidjson::Value data;
			data.SetObject();
			doc.AddMember("data", data, allocator);
		}
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		doc.Accept(writer);
		std::string msg = buffer.GetString();
		evbuffer_add_printf(req->buffer_out, "%s", msg.c_str());
		evhtp_send_reply(req, EVHTP_RES_OK);
		return;
	}

	int ia = atoi(a);
	int ib = atoi(b);

	int ic = Buniss::sub(ia, ib);

	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember("code", 0, allocator);
	doc.AddMember("msg", "success", allocator);
	{
		rapidjson::Value data;
		data.SetObject();
//		{
//			rapidjson::Value args;
//			args.SetArray();
//			{
//				rapidjson::Value vk;
//				vk.SetObject();
//				vk.AddMember("a", ia, allocator);
//				args.PushBack(vk, allocator);
//			}
//			{
//				rapidjson::Value vk;
//				vk.SetObject();
//				vk.AddMember("b", ib, allocator);
//				args.PushBack(vk, allocator);
//			}
////			args.PushBack(ia, allocator);
////			args.PushBack(ib, allocator);
//			data.AddMember("args", args, allocator);
//		}
		data.AddMember("result", ic, allocator);
		doc.AddMember("data", data, allocator);
	}
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string msg = buffer.GetString();
	evbuffer_add_printf(req->buffer_out, "%s", msg.c_str());
	evhtp_send_reply(req, EVHTP_RES_OK);

	LOGD(msg);
}
