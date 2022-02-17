#pragma once

namespace s2 {

class S2Thread
{
public:
	using thread_id_t = DWORD;

public:
	S2Thread();
	virtual ~S2Thread();
	
	virtual void			Destroy();

	// 가급적 ThreadCreate() 함수를 사용바랍니다.
	virtual bool			CreateThread(int32_t threadPriority, const char* file);
	virtual void			DestroyThread();	
	void					OnThreadRunning();

	void					SuspendThread()
	{
		::SuspendThread(m_thread);
	}
	void					ResumeThread()
	{
		::ResumeThread(m_thread);
	}

	void					Waiting(uint32_t ui32Millseconds)
	{
		::WaitForSingleObject(m_thread, ui32Millseconds);
	}

	bool					IsThreadRunning()
	{ 
		return m_isRunning; 
	}

	thread_id_t				GetThreadId()
	{
		return m_threadID;
	}

protected:	
	virtual bool			OnThreadUpdate()
	{
		return false;
	}

protected:	
	HANDLE					m_thread = INVALID_HANDLE_VALUE;
	thread_id_t				m_threadID = 0;
	bool					m_isRunning = false;

#ifdef _DEBUG
public:
	std::string				m_callFunc;
#endif
};

#define ThreadCreate()		S2Thread::CreateThread(THREAD_PRIORITY_NORMAL, __FILE__)

}