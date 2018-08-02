#ifndef __lock_h__
#define __lock_h__

//#define _USE_SYS_LOCK
#define _USE_POSIX_LOCK

#if defined(_WIN32) || defined(WIN32)

//#	if _MSC_VER <= 1600
#		if defined(_USE_SYS_LOCK)
			#include <WinSock2.h>
			#include <Windows.h>
#		elif defined(_USE_POSIX_LOCK)
			#include <pthread.h>
#		else
			#include <Windows.h>
#		endif
//#	endif

#else

#	if defined(_USE_POSIX_LOCK)
#		include <pthread.h>
#	endif

#endif

#ifdef _USE_STDCPP_LOCK
#endif

class lw_fast_lock_abstract
{
public:
	lw_fast_lock_abstract() {}
	virtual ~lw_fast_lock_abstract(void) {}

private:
	lw_fast_lock_abstract(const lw_fast_lock_abstract&);
	lw_fast_lock_abstract& operator=(const lw_fast_lock_abstract&);

public:
	virtual void lock() = 0;
	virtual void unlock() = 0;
};

class lw_fast_lock_guard
{
public:
	lw_fast_lock_guard(lw_fast_lock_abstract& lock);
	~lw_fast_lock_guard(void);

private:
	lw_fast_lock_abstract& _lock;
};

class lw_fast_mutex : public lw_fast_lock_abstract
{
	friend class lw_fast_cond;
public:
	lw_fast_mutex();
	virtual ~lw_fast_mutex(void);

public:
	virtual void lock();
	virtual void unlock();

#if defined(_USE_SYS_LOCK)
	CRITICAL_SECTION _t;
#elif defined(_USE_POSIX_LOCK)
	pthread_mutex_t _t;
#elif defined(_USE_STDCPP_LOCK)
	std::mutex _t;
#else
	CRITICAL_SECTION _t;
#endif

};

class lw_fast_cs : public lw_fast_lock_abstract
{
public:
	lw_fast_cs();
	virtual ~lw_fast_cs(void);

public:
	virtual void lock();
	virtual void unlock();

#if defined(_USE_SYS_LOCK)
	CRITICAL_SECTION _t;
#elif defined(_USE_POSIX_LOCK)
#elif defined(_USE_STDCPP_LOCK)
#else
	CRITICAL_SECTION _t;
#endif
};

class lw_fast_cond {
public:
	lw_fast_cond();
	~lw_fast_cond();

public:
	void wait(lw_fast_mutex& lock);

public:
	void signal();
	void broadcast();

private:

#if defined(_USE_SYS_LOCK)
	CONDITION_VARIABLE  _t;
#elif defined(_USE_POSIX_LOCK)
	pthread_cond_t _t;
#elif defined(_USE_STDCPP_LOCK)
#else
	CONDITION_VARIABLE  _t;
#endif
};


#endif	//__lock_h__
