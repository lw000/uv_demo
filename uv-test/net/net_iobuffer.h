#ifndef __net_core_h__
#define __net_core_h__

#include <functional>
#include "net_cache_queue.h"
#include "net_packet.h"
#include "lock.h"

struct MSG;

typedef void(*PARSE_CALLFUNC)(MSG* pack, void* userdata);

struct MSG {
    int main_cmd;
    int assi_cmd;
    char* buf;
    int size;
};

class NetIOBuffer
{
public:
	NetIOBuffer();
	~NetIOBuffer();

public:
	int send(int main_cmd, int assi_cmd, void* buf, int size, std::function<int(NetPacket*)> func);
    
public:
	int parse(const char * buf, int size, PARSE_CALLFUNC func, void* userdata);
//    int parse(const char * buf, int size, std::function<int(MSG*, void* userdata)> func, void* userdata);
    
private:
	NetIOBuffer(const NetIOBuffer&);
	NetIOBuffer& operator=(const NetIOBuffer&);

private:
	CacheQueue	_cache;
	lw_fast_mutex _rlock;
};

#endif // !__net_core_h__


