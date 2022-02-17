#pragma once

namespace s2 {

class S2ThreadPool
{
public:
	S2ThreadPool();
	virtual ~S2ThreadPool();

	virtual bool			Create(int32_t threadCount);
	void					Destroy();

	S2Thread*				PopWaitingThread();
	bool					PushWaitingThread(S2Thread* thread);

protected:
	void					MakeWaitingList();

protected:
	std::vector<S2Thread*>	m_threadList;
	std::stack<S2Thread*>	m_waitingList;
};

}