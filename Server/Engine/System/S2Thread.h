#pragma once

#include "S2ThreadMgr.h"

namespace s2 {

class S2Thread
{
public:
	using thread_id_t = std::thread::id;

	S2Thread() {};
	virtual ~S2Thread() {};
	
	// 가급적 ThreadCreate() 함수를 사용바랍니다.
	template<typename Func>
	bool CreateThread(Func func, int32_t priority, const char* file, bool useLog)
	{
	#ifdef _DEBUG
		m_callFunc = file;
	#endif
		m_thread = std::thread(func);
	
		if(false == SubCreateThread(useLog))
		{
			return false;
		}
		return true;
	};
	void DestroyThread()
	{
		if(m_isRunning)
		{
			m_isRunning = false;
			m_thread.join();
		}
	}
	template<typename Func>
	void UpdateThread(Func func)
	{
		m_isRunning = true;
		while(m_isRunning)
		{
			if(false == func())
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}
	}
	thread_id_t	GetThreadId()
	{
		return m_thread.get_id();
	}
	bool SubCreateThread(bool useLog);

protected:	
	std::thread				m_thread;
	bool					m_isRunning = false;
#ifdef _DEBUG
public:
	std::string				m_callFunc;
#endif
};

}

#define THREAD_UPDATE_START		auto thread_update = [this]() -> bool {	bool isWorking = false;
#define THREAD_UPDATE_END		return isWorking;};	m_thread.UpdateThread(thread_update);

#define ThreadCreateFunc(f)		CreateThread(f, THREAD_PRIORITY_NORMAL, __FILE__, true)