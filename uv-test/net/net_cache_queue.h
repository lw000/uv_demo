#ifndef __cache_queue_h__
#define __cache_queue_h__

#include <vector>
#include <queue>
#include <stddef.h>

#ifdef __cplusplus 
extern "C" {
#endif
    
	#include "queue.h"

#ifdef __cplusplus
}
#endif

class CacheQueue
{
public:
	CacheQueue(void);
	~CacheQueue(void);

public:
	char* front();
	void push(char* buf, int size);
	void pop(int size);

public:
	void copyto(char* buffer, int size);
	std::vector<char>* copyto(std::vector<char>& dest, int size);

public:
	size_t size() const;
	void clear();

private:
	std::vector<char> _cq;
};

class NewCacheQueue {
public:
	NewCacheQueue(size_t capacity);
	~NewCacheQueue();

public:
	size_t size() const;
	void clear();

public:
	void push(char c);
	void push(const char* buf, int size);

	char* front();

	void pop();
	void pop(int size);

public:
	void copyto(char* buffer, int size);

private:
	queue_t * _queue;
};

#endif // !__cache_queue_h__

