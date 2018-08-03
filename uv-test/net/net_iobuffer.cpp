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

int NetIOBuffer::send(int main_cmd, int assi_cmd, void* object, int objectSize, std::function<int(NetPackage* p)> func) {
	int c = -1;
	{
		NetPackage* msg = new NetPackage(main_cmd, assi_cmd, object, objectSize);
//        NET_MESSAGE netMsg;
//        netMsg.main_cmd = msg->getHead()->msg_main_cmd;
//        netMsg.assi_cmd = msg->getHead()->msg_assi_cmd;
//        netMsg.buf = msg->getBuf();
//        netMsg.size = msg->getSize();
		{
			c = func(msg);
		}
		delete msg;
	}
	return c;
}

int NetIOBuffer::parse(const char * buf, int size, PARSE_DATA_CALLFUNC call, void* userdata)
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

	assert(NULL != call);
	if (NULL == call) {
		LOGFMTD("callback is null.");
		return -2;
	}

	int ret = -3;

// 	clock_t t = clock();

	{
		lw_fast_lock_guard l(_rlock);
		_cacheQueue.push(const_cast<char*>(buf), size);
		int tmpCacheLength = (int)_cacheQueue.size();

		if (tmpCacheLength < C_NETHEAD_SIZE) {
			return -3;
			LOGFMTD("not a complete data packet [cache_size:%d head_size:%d]", tmpCacheLength, C_NETHEAD_SIZE);
		}

		do
		{
			NetHead *nh = (NetHead*)_cacheQueue.front();

			if (tmpCacheLength < nh->msg_size) {
				LOGFMTD("not a complete data packet [cache_size:%d, head_size:%d]", tmpCacheLength, nh->msg_size);
				break;
			}

			NetPackage* nmsg = new NetPackage(nh);
			if (nullptr != nmsg) {
				char* buf = _cacheQueue.front();
				char* tbuf = &buf[C_NETHEAD_SIZE];
				int tbuf_len = nh->msg_size - C_NETHEAD_SIZE;
				nmsg->setMessage(tbuf, tbuf_len);
				call(nmsg->getHead()->msg_main_cmd, nmsg->getHead()->msg_assi_cmd, nmsg->getBuf(), nmsg->getSize(), userdata);
			}
			delete nmsg;

			_cacheQueue.pop(nh->msg_size);
			tmpCacheLength = (int)_cacheQueue.size();
		} while (tmpCacheLength >= C_NETHEAD_SIZE);

// 		clock_t t1 = clock();
// 		LOGFMTD("NetCore::parse time [%f]", ((double)t1 - t) / CLOCKS_PER_SEC);
	}
	return 0;
}
