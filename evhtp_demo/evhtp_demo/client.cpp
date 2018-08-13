//============================================================================
// Name        : evhtp_client.cpp
// Author      : lw
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "example.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <inttypes.h>
#include <evhtp/evhtp.h>

#include <log4z/log4z.h>
using namespace zsummer::log4z;

static void add_cb(evhtp_request_t * req, void * arg) {
	char buf[1024];
	struct evbuffer *evbuf = req->buffer_in;
	ev_ssize_t len = evbuffer_get_length(evbuf);
	memcpy(buf, evbuffer_pullup(evbuf, len), len);
	buf[len] = '\0';
	evbuffer_drain(evbuf, len);

	LOGD(buf);
}

static void fact_cb(evhtp_request_t * req, void * arg) {
	char buf[1024];
	struct evbuffer *evbuf = req->buffer_in;
	ev_ssize_t len = evbuffer_get_length(evbuf);
	memcpy(buf, evbuffer_pullup(evbuf, len), len);
	buf[len] = '\0';
	evbuffer_drain(evbuf, len);

	LOGD(buf);
}

int main_client(const char* host, int port) {
	evbase_t * evbase;

	evbase = event_base_new();

	{
		evhtp_connection_t * conn;
		evhtp_request_t * request;
		conn = evhtp_connection_new(evbase, host, port);
		request = evhtp_request_new(add_cb, evbase);
		evhtp_headers_add_header(request->headers_out, evhtp_header_new("Host", "ieatfood.net", 0, 0));
		evhtp_headers_add_header(request->headers_out, evhtp_header_new("User-Agent", "libevhtp", 0, 0));
		evhtp_headers_add_header(request->headers_out, evhtp_header_new("Connection", "close", 0, 0));
		evhtp_make_request(conn, request, htp_method_GET, "/add?a=111&b=222");
	}

	{
		evhtp_connection_t * conn;
		evhtp_request_t * request;
		conn = evhtp_connection_new(evbase, host, port);
		request = evhtp_request_new(fact_cb, evbase);
		evhtp_headers_add_header(request->headers_out, evhtp_header_new("Host", "ieatfood.net", 0, 0));
		evhtp_headers_add_header(request->headers_out, evhtp_header_new("User-Agent", "libevhtp", 0, 0));
		evhtp_headers_add_header(request->headers_out, evhtp_header_new("Connection", "close", 0, 0));
		evhtp_make_request(conn, request, htp_method_GET, "/fact?v=10");
	}

	event_base_dispatch(evbase);
	event_base_free(evbase);

	return 0;
}
