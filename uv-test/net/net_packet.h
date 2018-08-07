#ifndef __NetPackage_h__
#define __NetPackage_h__

#include <functional>
#include <iosfwd>
#include <iostream>
#include <string>

#pragma pack(1)

typedef struct NetPacketHead
{
	friend std::ostream& operator<<(std::ostream & os, NetPacketHead & o);

public:
	int	            size;			// 数据包大小
    int             main_cmd;       // 指令
    int             assi_cmd;       // 指令
	unsigned int    ctime;	        // 发送时间
    unsigned short  v;              // 通讯版本

public:
	NetPacketHead(int main_cmd, int assi_cmd);

public:
	std::string debug();

};

#pragma pack()

class NetPacket
{
public:
	NetPacket(const NetPacketHead* head);
    
public:
	~NetPacket();

public:
	int setBuffer(void* buf, int size);
    
public:
	char* Buffer() const;
	int BufferSize() const;
    
public:
    char* Data() const;
    int DataSize() const;
    
public:
	const NetPacketHead* PacketHead() const;

public:
	std::string debug();

private:
	NetPacketHead* _head;
	char *_buffer;
	unsigned int _size;
};

#endif	//__NetPackage_h__
