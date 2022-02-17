#include "S2Engine.h"
#include "S2IocpWorkerMgr.h"

namespace s2 {

S2IocpWorkerMgr::S2IocpWorkerMgr()
{
}

S2IocpWorkerMgr::~S2IocpWorkerMgr()
{
}

bool S2IocpWorkerMgr::Create(int32_t workerCount)
{
	for(int32_t i = 0 ; i < workerCount ; ++i)
	{
		S2IocpWorker* worker = new S2IocpWorker(i);
		if(nullptr == worker)
		{
			return false;
		}
		if(false == worker->Create())
		{
			return false;
		}
		m_workerList.push_back(worker);;
	}
	return true;
}

void S2IocpWorkerMgr::Destroy()
{
	for(auto& iter : m_workerList)
	{
		iter->Destroy();
		SAFE_DELETE(iter);
	}
}

bool S2IocpWorkerMgr::Init(HANDLE IOCP)
{
	for(auto& iter : m_workerList)
	{
		iter->SetIOCP(IOCP);
	}
	return true;
}

}