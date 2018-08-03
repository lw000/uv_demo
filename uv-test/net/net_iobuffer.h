#ifndef __net_core_h__
#define __net_core_h__

#include <functional>
#include "cache_queue.h"
#include "lock.h"
#include "net_package.h"

struct MSG;

typedef void(*PARSE_DATA_CALLFUNC)(MSG* pack, void* userdata);

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
	int send(int main_cmd, int assi_cmd, void* buf, int size, std::function<int(NetPackage* p)> func);
    
public:
	int parse(const char * buf, int size, PARSE_DATA_CALLFUNC func, void* userdata);
	
private:
	NetIOBuffer(const NetIOBuffer&);
	NetIOBuffer& operator=(const NetIOBuffer&);

private:
	CacheQueue	_cache;
	lw_fast_mutex _rlock;
};

#endif // !__net_core_h__


