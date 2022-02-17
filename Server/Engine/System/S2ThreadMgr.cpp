#include "S2Engine.h"
#include "S2ThreadMgr.h"

namespace s2 {

S2ThreadMgr::S2ThreadMgr()
{
}

S2ThreadMgr::~S2ThreadMgr()
{
}

bool S2ThreadMgr::PushThread(const S2Thread::thread_id_t& id, const S2Thread* thread)
{
	// Push 가 잠겨 더이상 추가 불가능
	if(m_registLock)
	{
		S2LOG("RegistLock Error");
		return false;
	}

	auto result = m_threadList.emplace(std::make_pair(id, thread));
	return result.second;
}

bool S2ThreadMgr::PopThread(const S2Thread::thread_id_t& id)
{
	// Push 가 잠겨 더이상 제거 불가능
	if (m_registLock)
	{
		S2LOG("RegistLock Error");
		return false;
	}

	auto iter = m_threadList.find(id);
	if(iter == m_threadList.end())
		return false;

	m_threadList.erase(id);

	return true;
}

}