#include "net_packet.h"

#include <time.h>
#include <stdlib.h>
#include <string.h>

static const int C_NETHEAD_SIZE = sizeof(NetHead);

//////////////////////////////////////////////////////////////////////////////////////
static unsigned short __make_package_version(unsigned char major, unsigned char minor) {
	unsigned int version = major;
	version = (version << 8);
	version = (version) | minor;
	return version;
}

static void __package_version(unsigned short version, unsigned char &major, unsigned char &minor) {
	minor = (version & 0x00FF);

	version = (version >> 8);
	major = (version & 0x00FF);
}

std::ostream& operator<<(std::ostream & os, _tagNetHead & o)
{
	os << o.debug();
	return os;
}

_tagNetHead::_tagNetHead()
{
	this->size = -1;			// 数据包大小
    this->main_cmd = -1;        //
    this->assi_cmd = -1;        //
	this->ctime = 0;		    // 发送时间
	this->v = __make_package_version(1, 1);
}

std::string _tagNetHead::debug()
{
	char buf[512];
	unsigned char v1;
	unsigned char v2;
	__package_version(this->v, v1, v2);
	sprintf(buf, "size:%d, time:%u, v:%d.%d", this->size, this->ctime, v1, v2);
	return std::string(buf);
}

//////////////////////////////////////////////////////////////////////////////////////

NetPacket::NetPacket() : _size(0), _buf(NULL)
{
	this->_buf = NULL;
	this->_head = new NetHead;
}

NetPacket::NetPacket(int main_cmd, int assi_cmd, void* buf, int size) : _size(0), _buf(NULL)
{
    this->_head = new NetHead;
    this->setMessage(main_cmd, assi_cmd, buf, size);
}

NetPacket::NetPacket(const NetHead* head) : _size(0), _buf(NULL)
{
	if (head != NULL) {
		this->_head = new NetHead;
		*this->_head = *head;
	}
	else {

	}
}

NetPacket::~NetPacket()
{
	if (this->_buf != NULL)
	{
		free(this->_buf);
		this->_buf = NULL;
	}

	if (this->_head != NULL)
	{
		delete this->_head;
		this->_head = NULL;
	}
}

int NetPacket::setMessage(int main_cmd, int assi_cmd, void* buf, int size)
{
	if ((buf != NULL) && (size <= 0)) {
		return -1;
	}

	this->_head->size = size + C_NETHEAD_SIZE;
    this->_head->main_cmd = main_cmd;
    this->_head->assi_cmd = assi_cmd;
	this->_head->ctime = (unsigned int)time(NULL);
	
    this->_size = this->_head->size;
    this->_buf = (char*)malloc(this->_head->size * sizeof(char));
	::memcpy(this->_buf, this->_head, C_NETHEAD_SIZE);
	
	if ((NULL != buf) && (size > 0)) {
		::memcpy(&this->_buf[C_NETHEAD_SIZE], buf, size);
	}

	return 0;
}

std::string NetPacket::debug()
{
	char buf[256];
	sprintf(buf, "NetPackage: %s", this->_head->debug().c_str());
	return std::string(buf);
}

char* NetPacket::getBuf() const
{
	return this->_buf;
}

int NetPacket::getSize() const
{
	return this->_size;
}

const NetHead* NetPacket::getHead() const
{
	return this->_head;
}
