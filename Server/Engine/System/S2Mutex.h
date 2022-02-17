#pragma once

#include <mutex>

namespace s2 {

class S2Mutex : std::mutex
{
public:
	S2Mutex() = default;
	~S2Mutex() = default;
};

class S2Scoped_Mutex : std::lock_guard<std::mutex>
{
public:
	S2Scoped_Mutex(std::mutex& spinLock)
		: std::lock_guard<std::mutex>(spinLock)
	{
	}
	~S2Scoped_Mutex() = default;
};

}
//std::lock_guard<std::mutex> mutex(m_mutex);