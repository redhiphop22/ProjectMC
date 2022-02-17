#pragma once

namespace s2 {

class S2ThreadMgr : public S2Singleton<S2ThreadMgr>
{
public:
	S2ThreadMgr();
	~S2ThreadMgr();

	bool					PushThread(const S2Thread::thread_id_t& id, const S2Thread* thread);
	bool					PopThread(const S2Thread::thread_id_t& id);

	void					SetRegistLock()	{ m_registLock = true;	}

private:
	bool					m_registLock = false;
	std::unordered_map<S2Thread::thread_id_t, const S2Thread*> m_threadList;
};

}
#define S2THREAD_INSTANCE()		s2::S2ThreadMgr::GetInstance()