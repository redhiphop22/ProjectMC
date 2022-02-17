#include "S2Engine.h"
#include "S2Thread.h"

namespace s2 {

void threadProc(S2Thread* thread)
{
#ifdef _DEBUG
	std::string callFunc = thread->m_callFunc;
#endif
	thread->OnThreadRunning();
}

S2Thread::S2Thread()
{
}

S2Thread::~S2Thread()
{
}

void S2Thread::Destroy()
{
	DestroyThread();
	::CloseHandle(m_thread);
	m_thread = NULL;
}

bool S2Thread::CreateThread(int32_t priority, const char* file)
{
#ifdef _DEBUG
	m_callFunc = file;
#endif

	m_thread = ::CreateThread(NULL, 4096, (LPTHREAD_START_ROUTINE)threadProc, this, 0, &m_threadID);
	if(NULL == m_thread)
		return false;

	if(FALSE == SetThreadPriority(m_thread, priority))
		return false;
	
	if(false == S2LOG_INSTANCE().PushThread(m_threadID))
	{
		S2LOG("Log Mgr Error");
		return false;
	}

	if(false == S2THREAD_INSTANCE().PushThread(m_threadID, this))
	{
		// 에러 로그만
		S2LOG("Thread Mgr Error");
	}

	return true;
};

void S2Thread::DestroyThread()
{
	if(m_isRunning)
	{
		m_isRunning = false;
		::WaitForSingleObject(m_thread, 0xFFFFFFFF);
	}
}

void S2Thread::OnThreadRunning()
{
	m_isRunning = true;
	while(m_isRunning)
	{
		if(false == OnThreadUpdate())
		{
			::WaitForSingleObject(m_thread, 1);
		}
	}
}

}