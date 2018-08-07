#include "TCPServer.h"
#include <assert.h>

#include "Utils.h"

/*
ws2_32.lib
IPHLPAPI.lib
Psapi.lib
userenv.lib
*/

#if defined(_WIN32) || defined(WIN32)
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "userenv.lib")
#pragma comment(lib, "libuv-1.4.2.lib")
#endif

namespace lw
{
	///////////////////////////////////////////////////////////////////////////////////////////
    
    struct SharedData {
        TCPServer* srv;
        uv_stream_s* cli;
    };
    
	class UVWrapper
	{
	public:
        UVWrapper() {
        }
        
        ~UVWrapper() {
        }

	public:
        static void parse_cb(MSG* pack, void* userdata) {
            SharedData * data = (SharedData*)userdata;
            data->srv->onMessage(data->cli, pack);
        }
        
        static void idle_cb(uv_idle_t* handle)
        {
            TCPServer * srv = (TCPServer*)handle->data;
            srv->onIdle();
        }
        
        static void close_cb(uv_handle_t* handle)
        {
            TCPServer * srv = (TCPServer*)handle->data;
            srv->onClientClose(handle);
        }
        
        static void timer_cb(uv_timer_t* handle)
        {
            TCPServer * srv = (TCPServer*)handle->data;
            srv->onTimer();
        }
        
        static void on_resolved(uv_getaddrinfo_t *req, int status, struct addrinfo *res)
        {
            TCPServer * srv = (TCPServer*)req->data;
            srv->onResolved(status, res);
            uv_freeaddrinfo(res);
        }
        
        static void alloc_buffer_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
        {
            TCPServer * srv = (TCPServer*)handle->data;
            srv->onAllocBuffer(suggested_size, buf);
        }
        
        static void read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
        {
            if (nread > 0)
            {
                TCPServer * srv = (TCPServer*)stream->data;
                srv->onRead(stream, nread, buf);
            }
            else if (nread == 0)
            {
                printf("server close. \n");
            }
            else
            {
                printf("read_cb error: %s\n", uv_err_name(nread));
                
                assert(nread == UV_ECONNRESET || nread == UV_EOF);
                
                uv_close((uv_handle_t*)stream, close_cb);
            }
        }
        
        static void write_cb(uv_write_t *req, int status)
        {
            TCPServer * srv = (TCPServer*)req->data;
            srv->onAfterWrite(req, status);
        }
        
        static void connection_cb(uv_stream_t* server, int status)
        {
            TCPServer * srv = (TCPServer*)server->data;
            srv->onConnect(server, status);
        }
        
	};
    
    ///////////////////////////////////////////////////////////////////////////////////////////
    
    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    
	TCPServer::TCPServer(void)
	{
//        this->_loop = uv_default_loop();
        this->_loop = uv_loop_new();

		uv_tcp_init(this->_loop, &this->_tcp);
		this->_tcp.data = this;
        
        uv_mutex_init(&this->_mutex);
	}

	TCPServer::~TCPServer(void)
	{
		uv_mutex_destroy(&this->_mutex);

		for (auto& c : this->_clients)
		{
			free(c.second);
		}
	}

    uv_loop_t* TCPServer::getLoop() {
        return this->_loop;
    }
    
    void TCPServer::extSrv() {
        {
            int ret = uv_idle_init(this->_loop, &this->_idle);
            this->_idle.data = this;
            ret = uv_idle_start(&this->_idle, UVWrapper::idle_cb);
            if (ret) {
                
            }
        }
        
        {
            int ret = uv_timer_init(this->_loop, &this->_timer);
            this->_timer.data = this;
            ret = uv_timer_start(&_timer, UVWrapper::timer_cb, 3000, 1);
            if (ret) {
                
            }
        }
    }
    
	int TCPServer::run(const char* host, const char* port)
	{
        this->extSrv();
        
        struct addrinfo hints;
        hints.ai_family = PF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = 0;
        
        uv_getaddrinfo_t resolver = {0};
        resolver.data = (void*)this;

        int r = uv_getaddrinfo(this->_loop, &resolver, UVWrapper::on_resolved, host, port, &hints);
        if (0 == r)
        {
            r = uv_run(this->_loop, UV_RUN_DEFAULT);
        }
        else
        {
            fprintf(stderr, "getaddrinfo call error %s\n", uv_err_name(r));
        }
        
        return r;
	}

	int TCPServer::run(const char* ip, unsigned int port)
	{
        this->extSrv();
        
		sockaddr_in addr;
		int r = uv_ip4_addr(ip, port, &addr);
		r = uv_tcp_bind(&this->_tcp, (const sockaddr*)&addr, 0);
		r = uv_listen((uv_stream_t*)&this->_tcp, 100, UVWrapper::connection_cb);
        return uv_run(this->_loop, UV_RUN_DEFAULT);
	}

	void TCPServer::onClientClose(uv_handle_t* client)
	{
		auto selector = this->_clients.find((uv_tcp_s*)client);
		if (selector != this->_clients.end())
		{
			free(selector->second);
			this->_clients.erase(selector);
		}
	}
    
    void TCPServer::onIdle() {
        
    }

	void TCPServer::onTimer()
	{
		for (auto c : this->_clients)
		{
//            sendTCPData(c.second, mainid_net_connect, assid_net_test, 0, 0, nullptr, 0);
		}
	}

	void TCPServer::onResolved(int status, struct addrinfo *res)
	{
		char addr[16+1] = {0};
		int ret = uv_ip4_name((struct sockaddr_in*)res->ai_addr, addr, sizeof(addr));
		printf("ip : %s\n", addr);

		//ret = uv_timer_start(&_timer, _timer_cb, 3000, 1);

		ret = uv_tcp_bind(&this->_tcp, (const sockaddr*)&addr, 1);
		ret = uv_listen((uv_stream_t*)&this->_tcp, 1024, UVWrapper::connection_cb);
	}
	
	void TCPServer::onAllocBuffer(size_t suggested_size, uv_buf_t* buf)
	{
		buf->base = (char*)malloc(suggested_size);
		buf->len = suggested_size;
	}

	int TCPServer::sendData(uv_tcp_t* cli, unsigned int main_cmd, unsigned int assi_cmd, void* buf, int size)
	{
        this->_ioBuffer.send(main_cmd, assi_cmd, buf, size, [this, cli](NetPacket* pkt) -> int {
            uv_write_t *req = (uv_write_t*)malloc(sizeof(uv_write_t));
            req->data = this;

            uv_buf_t buft = uv_buf_init(pkt->Buffer(), pkt->BufferSize());
        
            int c = uv_write(req, (uv_stream_t*)cli, &buft, 1, UVWrapper::write_cb);
            if (c == 0) {
                
            } else {
                
            }
            return c;
        });
        
        return 0;
	}

	void TCPServer::onRead(uv_stream_t* cli, ssize_t nread, const uv_buf_t* buf)
	{
        SharedData shared_data;
        shared_data.srv = this;
        shared_data.cli = cli;
        this->_ioBuffer.parse(buf->base, nread, UVWrapper::parse_cb, &shared_data);
        
        free(buf->base);
	}

	void TCPServer::onAfterWrite(uv_write_t *req, int status)
	{
        if (req != NULL) {
            free(req);
        }
	}

	void TCPServer::onConnect(uv_stream_t* server, int status)
	{
		if (0 == status)
		{
			uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
			uv_tcp_init(this->_loop, client);
			client->data = this;

			_clients.insert(std::make_pair(client, client));

			if (uv_accept(server, (uv_stream_t*)client) == 0)
			{
                {
                    sockaddr_in addr;
                    int len = sizeof(addr);
                    //uv_tcp_getsockname(client, (struct sockaddr*)&addr, &len);
                    uv_tcp_getpeername(client, (struct sockaddr*)&addr, &len);
                    char clientIP[17];
                    uv_ip4_name(&addr, clientIP, 17);
                    printf("client : %s, port:%d\n", clientIP, addr.sin_port);
                    //char* p = inet_ntoa(addr.sin_addr);
                }

				int ret = uv_read_start((uv_stream_t*)client, UVWrapper::alloc_buffer_cb, UVWrapper::read_cb);
				if (0 == ret)
				{
                    
				}
			}
			else 
			{
				uv_close((uv_handle_t*)client, UVWrapper::close_cb);
			}
		}
		else
		{
			printf("socket error¡£[%d]\n", status);
		}
	}
}
