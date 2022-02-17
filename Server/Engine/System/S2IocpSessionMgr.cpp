#include "S2Engine.h"

namespace s2 {

S2IocpSessionMgr::S2IocpSessionMgr()
{
}

S2IocpSessionMgr::~S2IocpSessionMgr()
{
}

bool S2IocpSessionMgr::Create(int32_t sessionCount)
{
	for(int32_t i = 0 ; i < sessionCount ; ++i)
	{
		S2IocpSession* session = new S2IocpSession(i);
		if(nullptr == session)
		{
			return false;
		}
		auto result = m_sessionList.insert(std::make_pair(i, std::move(session)));
		if(false == result.second)
		{
			return false;
		}
	}

	return true;
}

void S2IocpSessionMgr::Destroy()
{
	for(auto& iter : m_sessionList)
	{
		SAFE_DELETE(iter.second);
	}
}

bool S2IocpSessionMgr::Init(HANDLE IOCP, SOCKET listenSocket)
{
	for(auto& iter : m_sessionList)
	{
		if(false == iter.second->Create(IOCP, listenSocket))
		{
			return false;
		}
	}
	return true;
}

}