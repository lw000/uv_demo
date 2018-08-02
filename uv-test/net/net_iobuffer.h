#ifndef __net_core_h__
#define __net_core_h__

#include <functional>
#include "cache_queue.h"
#include "lock.h"

typedef void(*PARSE_DATA_CALLFUNC)(int main_cmd, int assi_cmd, char* buf, int bufsize, void* userdata);

struct NET_MESSAGE
{
	char* buf;
	int size;
};

class NetIOBuffer
{
public:
	NetIOBuffer();
	~NetIOBuffer();

public:
	int send(int main_cmd, int assi_cmd, void* object, int objectSize, std::function<int(NET_MESSAGE* p)> func);
    
public:
	int parse(const char * buf, int size, PARSE_DATA_CALLFUNC call, void* userdata);
	
private:
	NetIOBuffer(const NetIOBuffer&);
	NetIOBuffer& operator=(const NetIOBuffer&);

private:
	CacheQueue	_cacheQueue;
	lw_fast_mutex _rlock;
};

#endif // !__net_core_h__


