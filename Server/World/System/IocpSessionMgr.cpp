#include "World.h"
#include "IocpSessionMgr.h"
#include "IocpSession.h"

IocpSessionMgr::IocpSessionMgr()
{
}

IocpSessionMgr::~IocpSessionMgr()
{
}

bool IocpSessionMgr::Create(int32_t sessionCount)
{
	for(int32_t i = 0 ; i < sessionCount ; ++i)
	{
		IocpSession* session = new IocpSession(i);
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
