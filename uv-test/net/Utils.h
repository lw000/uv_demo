#ifndef __UVHelperUtils_H__
#define __UVHelperUtils_H__

#include "uv.h"

namespace lw
{
	class Utils
	{
	public:
		class AutoMutex
		{
		public:
			AutoMutex(uv_mutex_t * mutex) : _mutex(mutex) { uv_mutex_init(_mutex); }
			~AutoMutex() { uv_mutex_destroy(_mutex);}
		private:
			uv_mutex_t * _mutex;
		};

	public:
		Utils(void);
		~Utils(void);
	};

}

#endif	//__UVHelperUtils_H__
