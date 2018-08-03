#ifndef __TCPServer_H__
#define __TCPServer_H__


#include <list>
#include <unordered_map>

#include "uv.h"

#include "net_package.h"
#include "net_iobuffer.h"


namespace lw
{
	class UVWrapper;

	class TCPServer
	{	
		friend UVWrapper;

    public:
        uv_loop_t * loop;
        
	private:
		UVWrapper* uvWrapper;
	
	private:
		uv_timer_t _timer;
		uv_tcp_t _tcp;	
		uv_idle_t _idle;
		uv_mutex_t _mutex;
		std::unordered_map<uv_tcp_t *, uv_tcp_t *> _clients;

	private:
		NetIOBuffer _ioBuffer;

	public:
		TCPServer(void);
		virtual ~TCPServer(void);

	public:
		void syncStart(const char* ip, unsigned int port);
		void syncStart(const char* host, const char* port);
        
    public:
        void asyncStart(const char* ip, unsigned int port);
        void asyncStart(const char* host, const char* port);
        
	public:
		int sendData(uv_tcp_s* cli, unsigned int main_cmd, unsigned int assi_cmd, void* buf, int size);

	public:
		virtual void onMessage(uv_stream_t* client, NetPackage* message) = 0;
        virtual void onStatus(int status) = 0;
        
	private:
		void onAllocBuffer(size_t suggested_size, uv_buf_t* buf);
		void onRead(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf);
		void onAfterWrite(uv_write_t *req, int status);
		void onConnect(uv_stream_t* server, int status);
		void onResolved(int status, struct addrinfo *res);
		void onTimer();
		void onClientClose(uv_handle_t* client);
		void onIdle();
        void onParse(NetPackage* msg, uv_stream_t* client);
        
    private:
        void extSrv();
	};
}

#endif	//__HN_TCPServer_H__
