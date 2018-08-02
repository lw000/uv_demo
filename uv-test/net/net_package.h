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
	int	            msg_size;			// 数据包大小
	unsigned short  msg_v;	            // 通讯版本
    int             msg_main_cmd;       // 指令
    int             msg_assi_cmd;       // 指令
	unsigned int    msg_ctime;	        // 发送时间

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
    NetPackage(int main_cmd, int assi_cmd, void* msg = nullptr, int size = 0);
//    NetPackage(void* msg = nullptr, int size = 0);
    
public:
	~NetPackage();

public:
	void setMessage(char* msg, int size);
	int setMessage(int main_cmd, int assi_cmd, void* msg, int size);
//    int setMessage(void* msg, int size);
    
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
