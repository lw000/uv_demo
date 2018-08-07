#include "TCPClient.h"
#include "Utils.h"
#include <assert.h>

/*
ws2_32.lib
IPHLPAPI.lib
Psapi.lib
userenv.lib
*/

#ifdef WIN32
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "userenv.lib")
#pragma comment(lib, "libuv-1.4.2.lib")
#pragma comment(lib, "MessagePack.lib")
#endif

namespace lw
{
    typedef struct {
        uv_write_t req;
        uv_buf_t buf;
    } write_req_t;
    
    static void free_write_req(uv_write_t *req) {
        write_req_t *wr = (write_req_t*) req;
        free(wr->buf.base);
        free(wr);
    }
    
	///////////////////////////////////////////////////////////////////////////////////////////
	static void _alloc_buffer_cb(uv_handle_t* handle, size_t suggested_size);
	static void _read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
	static void _write_cb(uv_write_t *req, int status);
	static void _connect_cb(uv_connect_t *req, int status);
	static void _on_resolved(uv_getaddrinfo_t *req, int status, struct addrinfo *res);
    static void parse_data_cb(MSG* pack, void* userdata);
    
	///////////////////////////////////////////////////////////////////////////////////////////

    static void parse_data_cb(MSG* pack, void* userdata) {
        TCPClient * cli = (TCPClient*)userdata;

        cli->onMessage(pack);
    }
    
	static void _on_resolved(uv_getaddrinfo_t *req, int status, struct addrinfo *res)
	{
		TCPClient * cli = (TCPClient*)req->data;
		cli->onResolved(status, res);
		uv_freeaddrinfo(res);
	}

	static void _alloc_buffer_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
	{
		TCPClient * cli = (TCPClient*)handle->data;
		cli->onAllocBuffer(suggested_size, buf);
	}

	static void _read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
	{                   
		if (nread > 0)
		{
			TCPClient * cli = (TCPClient*)stream->data;
			cli->onRead(nread, buf);
		}
		else if (nread == 0)
		{
			printf("server close\n");
		}
		else
		{
			printf("read_cb error:%s\n", uv_err_name(nread));
			assert(nread == UV_ECONNRESET || nread == UV_EOF);

			uv_close((uv_handle_t*)stream, nullptr);

			printf("read error\n");
		}
	}

	static void _write_cb(uv_write_t *req, int status)
	{            
		TCPClient * cli = (TCPClient*)req->data;
		cli->onWrite(req, status);
	}

	static void _connect_cb(uv_connect_t *req, int status)
	{
		TCPClient * cli = (TCPClient*)req->data;
		cli->onConnect(req->handle, status);
		free(req);
	}

    ////////////////////////////////////////////////////////////////////////////////////////////////
    
    TCPClient::TCPClient(void) : _delgate(NULL)
	{
        _loop = uv_loop_new();
        _cli = (uv_tcp_t*)::malloc(sizeof(uv_tcp_t));
		int ret = uv_tcp_init(_loop, _cli);
        if (ret == 0) {
            
        }
	}

    TCPClient::TCPClient(Delegate* delgate) {
        this->_delgate = delgate;
    }
    
	TCPClient::~TCPClient(void)
	{
        ::free(_cli);
        uv_loop_close(_loop);
	}

    void TCPClient::setDelgate(Delegate* delgate) {
        this->_delgate = delgate;
    }
    
    uv_loop_t* TCPClient::getloop() {
        return this->_loop;
    }
    
	int TCPClient::run(const char* host, const char* port)
	{
        struct addrinfo hints;
        hints.ai_family = PF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = 0;
        
        uv_getaddrinfo_t resolver = {0};
        resolver.data = this;

        int r = uv_getaddrinfo(this->_loop, &resolver, _on_resolved, host, port, &hints);
        if (0 == r) {
            r = uv_run(this->_loop, UV_RUN_DEFAULT);
        }
        else {
            fprintf(stderr, "getaddrinfo call error %s\n", uv_err_name(r));
        }
        
        return r;
	}

	int TCPClient::run(const char* ip, unsigned int port)
	{		
		uv_connect_t *connect_req = (uv_connect_t*) malloc(sizeof(uv_connect_t));	
		connect_req->data = (void*)this;

		sockaddr_in addr;
		int r = uv_ip4_addr(ip, port, &addr);
		r = uv_tcp_connect(connect_req, this->_cli, (const sockaddr*)&addr, _connect_cb);
		
		return uv_run(this->_loop, UV_RUN_DEFAULT);
	}
    
    int TCPClient::sendData(unsigned int main_cmd, unsigned int assi_cmd, void* buf, int size)
    {
        _ioBuffer.send(main_cmd, assi_cmd, buf, size, [this](NetPacket* pkt) -> int {
            {
                uv_write_t *req = (uv_write_t*)malloc(sizeof(uv_write_t));
                req->data = this;
                uv_buf_t buf_t = uv_buf_init(pkt->Buffer(), pkt->BufferSize());
                
                int c = uv_write(req, (uv_stream_t*)_cli, &buf_t, 1, _write_cb);
                if (c == 0) {
                    
                } else {
                    
                }
                return c;
            }
            
//            {
//                write_req_t * req = (write_req_t*)::malloc(sizeof(write_req_t));
//                req->buf = uv_buf_init(pkt->Buffer(), pkt->BufferSize());
//                int c = uv_write((uv_write_t*)req, (uv_stream_t*)_cli, &req->buf, 1, _write_cb);
//                if (c == 0) {
//                    
//                } else {
//                    
//                }
//                return c;
//            }
            
        });
        
        return 0;
    }
    
	void TCPClient::onAllocBuffer(size_t suggested_size, uv_buf_t* buf)
	{
        buf->base = (char*)malloc(suggested_size);
        buf->len = suggested_size;
	}

	void TCPClient::onRead(ssize_t nread, const uv_buf_t* buf)
	{
		if (nread > 0)
		{
            this->_ioBuffer.parse(buf->base, nread, parse_data_cb, this);
            if (buf->base != NULL) {
                free(buf->base);
            }
		}
		else if (nread == 0)
		{
			
		}
		else
		{
			
		}
	}

	void TCPClient::onWrite(uv_write_t *req, int status)
	{
        if (req != NULL) {
            free(req);
        }
	}

	void TCPClient::onResolved(int status, struct addrinfo *res)
	{
		char addr[17] = {0};
		uv_ip4_name((struct sockaddr_in*)res->ai_addr, addr, sizeof(addr)/sizeof(addr[0]));
	
		uv_connect_t* connect_req = (uv_connect_t*) malloc(sizeof(uv_connect_t));
		connect_req->data = this;
		int ret = uv_tcp_connect(connect_req, this->_cli, res->ai_addr, _connect_cb);
        if (ret == 0) {
            
        }
	}
    
	void TCPClient::onConnect(uv_stream_t* handle, int status)
	{
		if (0 == status)
		{
			handle->data = this;
			int r = uv_read_start(handle, _alloc_buffer_cb, _read_cb);
            if (r == 0) {
                this->onStatus(status);
            }
		}
		else
		{
			printf("socket error. [%d]\n", status);
		}
	}
	
	
}
