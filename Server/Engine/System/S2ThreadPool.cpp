#include "S2Engine.h"

namespace s2 {

S2ThreadPool::S2ThreadPool()
{
}

S2ThreadPool::~S2ThreadPool()
{
}

bool S2ThreadPool::Create(int32_t threadCount)
{
	for(int32_t i = 0 ; i < threadCount ; ++i)
	{
		S2Thread* thread = new S2Thread;
		if(nullptr == thread)
		{
			return false;
		}

		m_threadList.push_back(thread);
	}
	MakeWaitingList();
	return true;
}

void S2ThreadPool::Destroy()
{
	for(auto& iter : m_threadList)
	{
		iter->DestroyThread();
		delete iter;
	}
}

void S2ThreadPool::MakeWaitingList()
{
	// 역으로 넣습니다.
	for(auto iter = m_threadList.rbegin() ; iter != m_threadList.rend() ; iter++)
	{
		m_waitingList.push(*iter);
	}
}

S2Thread* S2ThreadPool::PopWaitingThread()
{
	if(m_waitingList.empty())
		return nullptr;

	auto& thread = m_waitingList.top();
	m_waitingList.pop();
	return thread;
}

bool S2ThreadPool::PushWaitingThread(S2Thread* thread)
{
	m_waitingList.push(thread);

	return true;
}

}