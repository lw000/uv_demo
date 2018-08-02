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
#pragma comment(lib, "MessagePack.lib")
#endif

namespace lw
{
	///////////////////////////////////////////////////////////////////////////////////////////
	class UVWrapper
	{
		TCPServer* server;

	public:
		UVWrapper(TCPServer* server);
		~UVWrapper();

	public:
		static void connection_cb(uv_stream_t* server, int status);
		static void on_resolved(uv_getaddrinfo_t *req, int status, struct addrinfo *res);
		static void alloc_buffer_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
		static void read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
		static void write_cb(uv_write_t *req, int status);
		static void timer_cb(uv_timer_t* handle);
		static void close_cb(uv_handle_t* handle);
		static void idle_cb(uv_idle_t* handle);
	};

	UVWrapper::UVWrapper(TCPServer* server) : server(server)
	{
	}

	UVWrapper::~UVWrapper()
	{
	}

	void UVWrapper::idle_cb(uv_idle_t* handle)
	{
		TCPServer * tcpServer = (TCPServer*)handle->data;
		tcpServer->onIdle();
	}

	void UVWrapper::close_cb(uv_handle_t* handle)
	{
		TCPServer * tcpServer = (TCPServer*)handle->data;
		tcpServer->onClientClose(handle);
	}

	void UVWrapper::timer_cb(uv_timer_t* handle)
	{
		TCPServer * tcpServer = (TCPServer*)handle->data;
		tcpServer->onTimer();
	}

	void UVWrapper::on_resolved(uv_getaddrinfo_t *req, int status, struct addrinfo *res)
	{
		TCPServer * tcpServer = (TCPServer*)req->data;
		tcpServer->onResolved(status, res);
		uv_freeaddrinfo(res);
	}

	void UVWrapper::alloc_buffer_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
	{
		TCPServer * tcpServer = (TCPServer*)handle->data;
		tcpServer->onAllocBuffer(suggested_size, buf);
	}

	void UVWrapper::read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
	{     
		if (nread > 0)
		{
			TCPServer * tcpServer = (TCPServer*)stream->data;
			tcpServer->onRead(stream, nread, buf);
		
		}
		else if (nread == 0)
		{
			printf("server close\n");
		}
		else
		{
			printf("read_cb error:%s\n", uv_err_name(nread));
			assert(nread == UV_ECONNRESET || nread == UV_EOF);

			uv_close((uv_handle_t*)stream, close_cb);
		}
	}

	void UVWrapper::write_cb(uv_write_t *req, int status)
	{            
		TCPServer * tcpServer = (TCPServer*)req->data;
		tcpServer->onAfterWrite(req, status);
	}

	void UVWrapper::connection_cb(uv_stream_t* server, int status)
	{
		TCPServer * tcpServer = (TCPServer*)server->data;
		tcpServer->onConnect(server, status);
	}
    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    
	TCPServer::TCPServer(void)
	{
		uv_mutex_init(&_mutex);

		uv_loop = uv_default_loop();
		//uv_loop = uv_loop_new();

		uv_tcp_init(uv_loop, &_tcp);
		_tcp.data = this;
	}

	TCPServer::~TCPServer(void)
	{
		uv_mutex_destroy(&_mutex);

		for (auto& c : _clients)
		{
			free(c.second);
		}
	}

    void TCPServer::extSrv() {
        {
            int ret = uv_idle_init(uv_loop, &_idle);
            _idle.data = this;
            ret = uv_idle_start(&_idle, UVWrapper::idle_cb);
            if (ret) {
                
            }
        }
        
        {
            int ret = uv_timer_init(uv_loop, &_uv_timer);
            _uv_timer.data = this;
            ret = uv_timer_start(&_uv_timer, UVWrapper::timer_cb, 3000, 1);
            if (ret) {
                
            }
        }
    }
    
	void TCPServer::syncStart(const char* host, const char* port)
	{
        this->extSrv();
        
        struct addrinfo hints;
        hints.ai_family = PF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = 0;
        
        uv_getaddrinfo_t resolver = {0};
        resolver.data = (void*)this;

		try
		{
			int ret = uv_getaddrinfo(uv_loop, &resolver, UVWrapper::on_resolved, host, port, &hints);

			if (0 == ret)
			{
				ret = uv_run(uv_loop, UV_RUN_DEFAULT);
			}
			else
			{			
				fprintf(stderr, "getaddrinfo call error %s\n", uv_err_name(ret));
			}			
		}
		catch(...) {}
	}

	void TCPServer::syncStart(const char* ip, unsigned int port)
	{
        this->extSrv();
        
		sockaddr_in addr;
		int ret = uv_ip4_addr(ip, port, &addr);

		ret = uv_tcp_bind(&_tcp, (const sockaddr*)&addr, 0);

		ret = uv_listen((uv_stream_t*)&_tcp, 100, UVWrapper::connection_cb);

		ret = uv_run(uv_loop, UV_RUN_DEFAULT);
	}
    
    void entry(void *arg) {
        TCPServer* srv = (TCPServer*)arg;
        int ret = uv_run(srv->uv_loop, UV_RUN_DEFAULT);
        if (ret == 0) {
            // .....
        } else {
            // .....
        }
    }
    
    void TCPServer::asyncStart(const char* ip, unsigned int port)
    {
        this->extSrv();
        
        sockaddr_in addr;
        int ret = uv_ip4_addr(ip, port, &addr);
        
        ret = uv_tcp_bind(&_tcp, (const sockaddr*)&addr, 0);
        
        ret = uv_listen((uv_stream_t*)&_tcp, 100, UVWrapper::connection_cb);
        
        uv_thread_t tid;
        uv_thread_create(&tid, entry, this);
    }
    
    void TCPServer::asyncStart(const char* host, const char* port)
    {
       this->extSrv();
        
        struct addrinfo hints;
        hints.ai_family = PF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = 0;
        
        uv_getaddrinfo_t resolver = {0};
        resolver.data = (void*)this;
        
        try
        {
            int ret = uv_getaddrinfo(uv_loop, &resolver, UVWrapper::on_resolved, host, port, &hints);
            
            if (0 == ret)
            {
                uv_thread_t tid;
                uv_thread_create(&tid, entry, this);
            }
            else
            {
                fprintf(stderr, "getaddrinfo call error %s\n", uv_err_name(ret));
            }
        }
        catch(...) {}
    }
    
	void TCPServer::onIdle()
	{

	}

	void TCPServer::onClientClose(uv_handle_t* client)
	{
		auto selector = _clients.find((uv_tcp_s*)client);
		if (selector != _clients.end())
		{
			free(selector->second);
			_clients.erase(selector);
		}
	}

	void TCPServer::onTimer()
	{
		for (auto c : _clients)
		{
//            sendTCPData(c.second, mainid_net_connect, assid_net_test, 0, 0, nullptr, 0);
		}
	}

	void TCPServer::onResolved(int status, struct addrinfo *res)
	{
		char addr[17] = {0};
		int ret = uv_ip4_name((struct sockaddr_in*)res->ai_addr, addr, sizeof(addr)/sizeof(addr[0]));
		printf("ip : %s\n", addr);

		//ret = uv_timer_start(&_timer, _timer_cb, 3000, 1);

		ret = uv_tcp_bind(&_tcp, (const sockaddr*)&addr, 1);
		ret = uv_listen((uv_stream_t*)&_tcp, 5, UVWrapper::connection_cb);
	}
	
	void TCPServer::onAllocBuffer(size_t suggested_size, uv_buf_t* buf)
	{
		buf->base = (char*)malloc(4096);
		buf->len = 4096;
	}

	int TCPServer::sendTCPData(uv_tcp_s* cli, unsigned int main_cmd, unsigned int assi_cmd, void* object, int objectSize)
	{
        _ioBuffer.send(main_cmd, assi_cmd, object, objectSize, [this, cli](NET_MESSAGE* msg) -> int {
            WriteData* writeData = new WriteData(this, msg->buf, msg->size);
            uv_write_t *req = (uv_write_t*)malloc(sizeof(uv_write_t));
            req->data = writeData;
            int c = uv_write(req, (uv_stream_s*)cli, writeData->getBuf(), 1, UVWrapper::write_cb);
            return c;
        });
        
        return 0;
	}

	void TCPServer::onRead(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf)
	{
		_ioBuffer.parse(buf->base, nread, NULL, NULL);
        
        free(buf->base);
	}

	void TCPServer::onAfterWrite(uv_write_t *req, int status)
	{
		WriteData* writeData = (WriteData*)req->data;
        if (writeData != NULL) {
            delete writeData;
        }
        
        if (req != NULL) {
            free(req);
        }
	}

	void TCPServer::onConnect(uv_stream_t* server, int status)
	{
		if (0 == status)
		{
			uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
			uv_tcp_init(uv_loop, client);
			client->data = this;

			auto clientPair = std::make_pair(client, client);
			_clients.insert(clientPair);

			if (uv_accept(server, (uv_stream_t*) client) == 0)
			{		
				sockaddr_in addr;
				int len = sizeof(addr);
				//uv_tcp_getsockname(client, (struct sockaddr*)&addr, &len);
				uv_tcp_getpeername(client, (struct sockaddr*)&addr, &len);
				char clientIP[17];
				uv_ip4_name(&addr, clientIP, 17);
				printf("client : %s, port:%d\n", clientIP, addr.sin_port);
				//char* p = inet_ntoa(addr.sin_addr);

				int ret = uv_read_start((uv_stream_t*)client, UVWrapper::alloc_buffer_cb, UVWrapper::read_cb);
				if (0 == ret)
				{
//                    MSG_S_ConnectSuccess ConnectSuccess = {0};
//                    ConnectSuccess.bLessVer = 3;
//                    ConnectSuccess.bMaxVer = 3;
//                    ConnectSuccess.i64CheckCode = 20130705;
//                    /*std::stringstream buf;
//                    msgpack::pack(buf, ConnectSuccess);*/
//                    sendTCPData(client, mainid_net_connect, assid_net_connect, 0, 0, &ConnectSuccess, sizeof(ConnectSuccess));
//                    //sendTCPData(client, 1, 1, 0, 0, buf.str().data(), buf.str().size());
//
//                    sendTCPData(client, mainid_net_connect, assid_net_test, 0, 0, nullptr, 0);
				}
			}
			else 
			{
				uv_close((uv_handle_t*) client, UVWrapper::close_cb);
			}
		}
		else
		{
			printf("socket error¡£[%d]\n", status);
		}
	}
}
