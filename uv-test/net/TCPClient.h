#ifndef __TCPClient_H__
#define __TCPClient_H__

#include <deque>
#include "net_iobuffer.h"
#include "net_package.h"

#include "uv.h"

namespace lw
{
    class Delegate
    {
    public:
        virtual ~Delegate() {}
        
    public:
        virtual void onMessage(NET_MESSAGE* msg) = 0;
    };

	class TCPClient
	{	
	public:
		class WriteData
		{
			TCPClient* cli;
			uv_buf_t buf_t;
		
		public:
			WriteData(TCPClient* cli, const char* buf, int len) : cli(cli)
			{
				buf_t.base = (char*)::malloc(len);
				buf_t.len = len;
				memcpy(buf_t.base, buf, len);
			}

			~WriteData()
			{
                if (buf_t.base != NULL) {
                    free(buf_t.base);
                }
			}

		public:
			uv_buf_t* getBuft() { return &buf_t; }
			TCPClient* getTCPClient() { return cli; }
		};
	
	private:
		uv_loop_t * _uv_loop;
		uv_tcp_t* _uv_cli;
		uv_mutex_t _mutex;

	private:
		NetIOBuffer _ioBuffer;

	public:
		TCPClient(void);
		virtual ~TCPClient(void);

	public:
		void syncStart(const char* ip, unsigned int port);
		void syncStart(const char* host, const char* port);

	public:
		int sendData(unsigned int main_cmd, unsigned int assi_cmd, void* object, int objectSize);

	public:
		virtual void onMessage(NET_MESSAGE* message) = 0;

	public:
		void onAllocBuffer(size_t suggested_size, uv_buf_t* buf);
		void onRead(ssize_t nread, const uv_buf_t* buf);
		void onWrite(uv_write_t *req, int status);
		void onConnect(uv_stream_t* handle, int status);
		void onResolved(int status, struct addrinfo *res);
        void onParse(int main_cmd, int assi_cmd, char* buf, int bufsize);
	};
}

#endif	//__TCPServer_H__
