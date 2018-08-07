#include "net_iobuffer.h"

#include <assert.h>
#include <string.h>

#include "log4z.h"

static const int C_NETPACKETHEAD_SIZE = sizeof(NetPacketHead);

//////////////////////////////////////////////////////////////////////////////////////////

NetIOBuffer::NetIOBuffer() {

}

NetIOBuffer::~NetIOBuffer() {

}

int NetIOBuffer::send(int main_cmd, int assi_cmd, void* buf, int size, std::function<int(NetPacket*)> func) {
    
    NetPacketHead packetHead(main_cmd, assi_cmd);
    NetPacket* pkt = new NetPacket(&packetHead);
    pkt->setBuffer(buf, size);
    int c = func(pkt);
    delete pkt;
    
	return c;
}

//int NetIOBuffer::parse(const char * buf, int size, std::function<int(MSG*, void* userdata)> func, void* userdata) {
//
//    return 0;
//}

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
		if (cache_size < C_NETPACKETHEAD_SIZE) {
			return -3;
			LOGFMTD("not a complete data packet [cache_size:%d head_size:%d]", cache_size, C_NETPACKETHEAD_SIZE);
		}

		do
		{
            NetPacketHead* nph = reinterpret_cast<NetPacketHead*>(_cache.front());
			if (cache_size < nph->size) {
				LOGFMTD("not a complete data packet [cache_size:%d, head_size:%d]", cache_size, nph->size);
				break;
			}

            char* cache_buf = _cache.front();
            char* tbuf = &cache_buf[C_NETPACKETHEAD_SIZE];
            int tbuf_size = nph->size - C_NETPACKETHEAD_SIZE;

            {
                NetPacket pkt(nph);
                pkt.setBuffer(tbuf, tbuf_size);
                
                MSG msg;
                msg.main_cmd = nph->main_cmd;
                msg.assi_cmd = nph->assi_cmd;
                msg.buf = tbuf;
                msg.size = tbuf_size;
                
                func(&msg, userdata);
            }
            
//            {
//                NetPacket pkt(nph);
//                pkt.setMessage(tbuf, tbuf_size);
//
//                MSG msg;
//                msg.main_cmd = pkt.PacketHead()->main_cmd;
//                msg.assi_cmd = pkt.PacketHead()->assi_cmd;
//                msg.buf = pkt.Data();
//                msg.size = pkt.DataSize();
//
//                func(&msg, userdata);
//            }

			_cache.pop(nph->size);
            
			cache_size = (int)_cache.size();
		} while (cache_size >= C_NETPACKETHEAD_SIZE);

// 		clock_t t1 = clock();
// 		LOGFMTD("NetCore::parse time [%f]", ((double)t1 - t) / CLOCKS_PER_SEC);
	}
	return 0;
}
