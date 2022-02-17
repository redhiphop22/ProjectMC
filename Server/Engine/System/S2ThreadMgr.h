#pragma once

namespace s2 {

class S2Thread;

class S2ThreadMgr : public S2Singleton<S2ThreadMgr>
{
public:
	S2ThreadMgr();
	~S2ThreadMgr();

	bool					PushThread(const std::thread::id& id, S2Thread* thread);
	bool					PopThread(const std::thread::id& id);

	void					SetRegistLock()	{ m_registLock = true;	}

private:
	bool					m_registLock = false;
	std::unordered_map<std::thread::id, const S2Thread*> m_threadList;
};

}
#define S2THREAD_INSTANCE()		s2::S2ThreadMgr::GetInstance()