#include "net_iobuffer.h"

#include <assert.h>
#include <string.h>

#include "log4z.h"

static const int C_NETHEAD_SIZE = sizeof(NetHead);

//////////////////////////////////////////////////////////////////////////////////////////

NetIOBuffer::NetIOBuffer() {

}

NetIOBuffer::~NetIOBuffer() {

}

int NetIOBuffer::send(int main_cmd, int assi_cmd, void* buf, int size, std::function<int(NetPacket*)> func) {
    
    NetPacket* pkt = new NetPacket(main_cmd, assi_cmd, buf, size);
    int c = func(pkt);
    delete pkt;
    
	return c;
}

int NetIOBuffer::parse(const char * buf, int size, PARSE_CALLFUNC func, void* userdata)
{
	assert(size > 0);
    
	if (size <= 0) {
		LOGFMTD("size:%d less than 0.", size);
		return -1;
	}
	
	assert(NULL != buf);
	if (NULL == buf) {
		LOGFMTD("buf is null.");
		return -1;
	}

	assert(NULL != func);
	if (NULL == func) {
		LOGFMTD("func is null.");
		return -2;
	}

// 	clock_t t = clock();

	{
		lw_fast_lock_guard l(_rlock);
        
		_cache.push(const_cast<char*>(buf), size);
        
		int cache_size = (int)_cache.size();
		if (cache_size < C_NETHEAD_SIZE) {
			return -3;
			LOGFMTD("not a complete data packet [cache_size:%d head_size:%d]", cache_size, C_NETHEAD_SIZE);
		}

		do
		{
			NetHead *nh = (NetHead*)_cache.front();
			if (cache_size < nh->size) {
				LOGFMTD("not a complete data packet [cache_size:%d, head_size:%d]", cache_size, nh->size);
				break;
			}

			NetPacket* pkt = new NetPacket(nh);
			if (nullptr != pkt) {
                char* buf = _cache.front();
                char* tbuf = &buf[C_NETHEAD_SIZE];
                int tbuf_len = nh->size - C_NETHEAD_SIZE;
                
                MSG msg;
                msg.main_cmd = pkt->getHead()->main_cmd;
                msg.assi_cmd = pkt->getHead()->assi_cmd;
                msg.buf = tbuf;
                msg.size = tbuf_len;
                
                func(&msg, userdata);
			}
			delete pkt;

			_cache.pop(nh->size);
            
			cache_size = (int)_cache.size();
		} while (cache_size >= C_NETHEAD_SIZE);

// 		clock_t t1 = clock();
// 		LOGFMTD("NetCore::parse time [%f]", ((double)t1 - t) / CLOCKS_PER_SEC);
	}
	return 0;
}
