#ifndef __NetPackage_h__
#define __NetPackage_h__

#include <functional>
#include <iosfwd>
#include <iostream>
#include <string>

static const int NET_HEART_BEAT_PING = -10000;
static const int NET_HEART_BEAT_PONG = -10001;

#pragma pack(1)

typedef struct _tagNetHead
{
	friend std::ostream& operator<<(std::ostream & os, _tagNetHead & o);

public:
	int	            size;			// 数据包大小
    int             main_cmd;       // 指令
    int             assi_cmd;       // 指令
	unsigned int    ctime;	        // 发送时间
    unsigned short  v;                // 通讯版本

public:
	_tagNetHead();

public:
	std::string debug();

} NetHead;

#pragma pack()

class NetPackage
{
public:
	NetPackage();
	NetPackage(const NetHead* head);
    NetPackage(int main_cmd, int assi_cmd, void* buf, int size);
    
public:
	~NetPackage();

public:
	int setMessage(int main_cmd, int assi_cmd, void* msg, int size);
    
public:
	char* getBuf() const;
	int getSize() const;
	const NetHead* getHead() const;

public:
	std::string debug();

private:
	NetHead* _head;
	char *_buf;
	unsigned int _size;
};

#endif	//__NetPackage_h__
