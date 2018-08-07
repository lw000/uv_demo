#ifndef __TCPClient_H__
#define __TCPClient_H__

#include <deque>
#include "net_iobuffer.h"
#include "net_packet.h"

#include "uv.h"

namespace lw
{
	class TCPClient
	{
    public:
        class Delegate
        {
        public:
            virtual ~Delegate() {}
            
        public:
            virtual void onMessage(MSG* pack) = 0;
            virtual void onStatus(int status) = 0;
        };
        
	private:
		uv_loop_t * _loop;
		uv_tcp_t* _cli;
//        uv_mutex_t _mutex;

    private:
        Delegate* _delgate;
        
	private:
		NetIOBuffer _ioBuffer;

	public:
		TCPClient(void);
        TCPClient(Delegate* delgate);
		
        virtual ~TCPClient(void);

    public:
        void setDelgate(Delegate* delgate);
        
    public:
        uv_loop_t* getloop();
        
	public:
		int run(const char* ip, unsigned int port);
		int run(const char* host, const char* port);
        
	public:
		int sendData(unsigned int main_cmd, unsigned int assi_cmd, void* buf, int size);

	public:
		virtual void onMessage(MSG* pack) = 0;
        virtual void onStatus(int status) = 0;
        
	public:
		void onAllocBuffer(size_t suggested_size, uv_buf_t* buf);
		void onRead(ssize_t nread, const uv_buf_t* buf);
		void onWrite(uv_write_t *req, int status);
		void onConnect(uv_stream_t* handle, int status);
		void onResolved(int status, struct addrinfo *res);
	};
}

#endif	//__TCPServer_H__
