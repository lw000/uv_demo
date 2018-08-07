#include "net_cache_queue.h"
#include <algorithm>

///////////////////////////////////////////////////////////////////////////////////////////
CacheQueue::CacheQueue(void) {
	
}
	
CacheQueue::~CacheQueue(void) {
	
}

void CacheQueue::push(char* buf, int size)
{
	if (buf == NULL) return;
	if (size <= 0) return;
	int i = 0;
	char* p = buf;
	while (i++ < size) {
		_cq.push_back(*p++);
	}
}

void CacheQueue::pop(int size)
{
	if (size <= 0) return;
	if (_cq.empty()) return;
	if (size > _cq.size()) return;
		
	std::vector<char>::iterator iter = _cq.erase(_cq.begin(), _cq.begin() + size);
    if (iter != _cq.end()) {
        
    }
}

void CacheQueue::copyto(char* buffer, int size) {
	if (_cq.size() < size) { 
		return;
	}

	char* p = buffer;
	int i = 0;
	std::for_each(_cq.begin(), _cq.end(), [&p, &i, &size](char c) {
		if (i++ < size) {
			*p++ = c;
		}
	});
}

std::vector<char>* CacheQueue::copyto(std::vector<char>& dest, int size) {
	if (_cq.size() < size)
	{
		return nullptr;
	}

	std::copy_n(_cq.begin(), size, dest.begin());

	return &dest;
}

size_t CacheQueue::size() const
{
	int l = 0;
	{
		l = _cq.size();
	}
	return l;
}

char* CacheQueue::front()
{
	return _cq.data();
}

void CacheQueue::clear()
{
	_cq.clear();
}


///////////////////////////////////////////////////////////////////////////////////////////
NewCacheQueue::NewCacheQueue(size_t capacity) {
	this->_queue = lw_queue_init(capacity);
}

NewCacheQueue::~NewCacheQueue() {
	this->clear();
	lw_queue_dispose(this->_queue);
}

void NewCacheQueue::push(char c) {
	lw_queue_add(this->_queue, new char(c));
}

void NewCacheQueue::push(const char* buf, int size) {
	for (int i = 0; i < size; i++) {
		lw_queue_add(this->_queue, new char(buf[i]));
	}
}

char* NewCacheQueue::front() {
	char* p = (char*)lw_queue_peek(this->_queue);
	return p;
}

void NewCacheQueue::pop() {
	char* p = (char*)lw_queue_remove(this->_queue);
	delete p;
}

void NewCacheQueue::pop(int size) {
	for (int i = 0; i < size; i++) {
		char* p = (char*)lw_queue_remove(this->_queue);
		delete p;
	}
}

void NewCacheQueue::copyto(char* buffer, int size) {
	for (int i = 0; i < size; i++) {
		char* p = (char*)lw_queue_remove(this->_queue);
		buffer[i] = *p;
		delete p;
	}
}

size_t NewCacheQueue::size() const {
	return this->_queue->size;
}

void NewCacheQueue::clear() {
	char* p = nullptr;
	while ((p = (char*)lw_queue_remove(this->_queue)) != nullptr)
	{
		delete p;
	}
}
