#include "lock.h"

#include <stdio.h>

lw_fast_lock_guard::lw_fast_lock_guard(lw_fast_lock_abstract& lock) : _lock(lock)
{
	this->_lock.lock();
}

lw_fast_lock_guard::~lw_fast_lock_guard(void)
{
	this->_lock.unlock();
}

//////////////////////////////////////////////////////////////////////////////////////////

lw_fast_mutex::lw_fast_mutex()
{
#if defined(_USE_SYS_LOCK)
	::InitializeCriticalSection(&this->_t);
#elif defined(_USE_POSIX_LOCK)
	pthread_mutex_init(&_t, NULL);
#endif
}

lw_fast_mutex::~lw_fast_mutex(void)
{
#if defined(_USE_SYS_LOCK)
	::DeleteCriticalSection(&this->_t);
#elif defined(_USE_POSIX_LOCK)
	pthread_mutex_destroy(&_t);
#endif
}

void lw_fast_mutex::lock()
{
#if defined(_USE_SYS_LOCK)
	::EnterCriticalSection(&this->_t);
#elif defined(_USE_POSIX_LOCK)
	pthread_mutex_lock(&_t);
#endif	
}

void lw_fast_mutex::unlock()
{
#if defined(_USE_SYS_LOCK)
	::LeaveCriticalSection(&this->_t);
#elif defined(_USE_POSIX_LOCK)
	pthread_mutex_unlock(&_t); 
#else

#endif
}

//////////////////////////////////////////////////////////////////////////////////////////

lw_fast_cs::lw_fast_cs()
{
#if defined(_USE_SYS_LOCK)
	::InitializeCriticalSection(&this->_t);
#elif defined(_USE_POSIX_LOCK)
	printf("only implemented in the Win32 operating system \n");
#endif
}

lw_fast_cs::~lw_fast_cs(void)
{
#if defined(_USE_SYS_LOCK)
	::DeleteCriticalSection(&this->_t);
#elif defined(_USE_POSIX_LOCK)
	printf("only implemented in the Win32 operating system \n");
#endif
}

void lw_fast_cs::lock()
{
#if defined(_USE_SYS_LOCK)
	::EnterCriticalSection(&this->_t);
#elif defined(_USE_POSIX_LOCK)
	printf("only implemented in the Win32 operating system \n");
#endif	
}

void lw_fast_cs::unlock()
{
#if defined(_USE_SYS_LOCK)
	::LeaveCriticalSection(&this->_t);
#elif defined(_USE_POSIX_LOCK)
	printf("only implemented in the Win32 operating system \n");
#else

#endif
}

//////////////////////////////////////////////////////////////////////////////////////////
lw_fast_cond::lw_fast_cond() {
#if defined(_USE_SYS_LOCK)
	InitializeConditionVariable(&_t);
#elif defined(_USE_POSIX_LOCK)
	pthread_cond_init(&_t, nullptr);
#else

#endif	
}

lw_fast_cond::~lw_fast_cond() {
#if defined(_USE_SYS_LOCK)

#elif defined(_USE_POSIX_LOCK)
	pthread_cond_destroy(&_t);
#else

#endif
	
}

void lw_fast_cond::wait(lw_fast_mutex& lock) {
#if defined(_USE_SYS_LOCK)
	SleepConditionVariableCS(&_t, &lock._t, INFINITE);
#elif defined(_USE_POSIX_LOCK)
	pthread_cond_wait(&_t, &lock._t);
#else

#endif
}

void lw_fast_cond::signal() {
#if defined(_USE_SYS_LOCK)
	WakeConditionVariable(&_t);  
#elif defined(_USE_POSIX_LOCK)
	pthread_cond_signal(&_t);
#else

#endif
	
}

void lw_fast_cond::broadcast() {
#if defined(_USE_SYS_LOCK)
	WakeAllConditionVariable(&_t);
#elif defined(_USE_POSIX_LOCK)
	pthread_cond_broadcast(&_t);
#else

#endif
	
}

