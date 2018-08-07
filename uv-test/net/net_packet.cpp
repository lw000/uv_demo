#include "net_packet.h"

#include <time.h>
#include <stdlib.h>
#include <string.h>

static const int C_NETPACKETHEAD_SIZE = sizeof(NetPacketHead);

//////////////////////////////////////////////////////////////////////////////////////
static unsigned short __make_packet_version(unsigned char major, unsigned char minor) {
	unsigned int version = major;
	version = (version << 8);
	version = (version) | minor;
	return version;
}

static void __packet_version(unsigned short version, unsigned char &major, unsigned char &minor) {
	minor = (version & 0x00FF);

	version = (version >> 8);
	major = (version & 0x00FF);
}

std::ostream& operator<<(std::ostream & os, NetPacketHead & o)
{
	os << o.debug();
	return os;
}

NetPacketHead::NetPacketHead(int main_cmd, int assi_cmd)
{
	this->size = 0;			        // 数据包大小
    this->main_cmd = main_cmd;          //
    this->assi_cmd = assi_cmd;          //
	this->ctime = (unsigned int)time(NULL);
	this->v = __make_packet_version(1, 1);
}

std::string NetPacketHead::debug()
{
	char buf[128];
	unsigned char v1;
	unsigned char v2;
	__packet_version(this->v, v1, v2);
	sprintf(buf, "size:%d, time:%u, v:%d.%d", this->size, this->ctime, v1, v2);
	return std::string(buf);
}

//////////////////////////////////////////////////////////////////////////////////////

NetPacket::NetPacket(const NetPacketHead* head)
{
    this->_size = 0;
    this->_buffer = NULL;
    if (head != NULL) {
        this->_head = new NetPacketHead(*head);
    }
}

NetPacket::~NetPacket()
{
	if (this->_buffer != NULL)
	{
		free(this->_buffer);
		this->_buffer = NULL;
	}

	if (this->_head != NULL)
	{
		delete this->_head;
		this->_head = NULL;
	}
}

int NetPacket::setBuffer(void* buf, int size)
{
	if ((buf != NULL) && (size <= 0)) {
		return -1;
	}

	this->_head->size = size + C_NETPACKETHEAD_SIZE;
    this->_size = this->_head->size;
    this->_buffer = (char*)malloc(this->_head->size * sizeof(char));
	::memcpy(this->_buffer, this->_head, C_NETPACKETHEAD_SIZE);
	
	if ((NULL != buf) && (size > 0)) {
		::memcpy(&this->_buffer[C_NETPACKETHEAD_SIZE], buf, size);
	}

	return 0;
}

std::string NetPacket::debug()
{
	char buf[256];
	sprintf(buf, "NetPackage: %s", this->_head->debug().c_str());
	return std::string(buf);
}

char* NetPacket::Buffer() const
{
	return this->_buffer;
}

int NetPacket::BufferSize() const
{
    return this->_size;
}

char* NetPacket::Data() const {
    return &this->_buffer[C_NETPACKETHEAD_SIZE];
}

int NetPacket::DataSize() const {
    return this->_size - C_NETPACKETHEAD_SIZE;
}

const NetPacketHead* NetPacket::PacketHead() const
{
	return this->_head;
}
