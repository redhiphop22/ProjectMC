#pragma once

namespace s2 {

class S2SpinLock
{
public:
	S2SpinLock() = default;
	~S2SpinLock() = default;

	void Lock()
	{
		while(lock.test_and_set(std::memory_order_acquire)) {}
	}
	bool TryLock()
	{
		return !lock.test_and_set(std::memory_order_acquire);
	}
	void Unlock()
	{
		lock.clear(std::memory_order_release);
	}

private:
	std::atomic_flag lock = ATOMIC_FLAG_INIT;
};

class S2Scoped_SpinLock
{
public:
	S2Scoped_SpinLock(S2SpinLock& spinLock)
		: m_spinLock(spinLock)
	{
		m_spinLock.Lock();
	}
	~S2Scoped_SpinLock()
	{
		m_spinLock.Unlock();
	}
private:
	S2SpinLock& m_spinLock;
};

}