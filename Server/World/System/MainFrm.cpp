#include "World.h"
#include "MainFrm.h"

MainFrm* mainFrm = nullptr;

bool MainFrm::OnInit()
{
	//if(false == S2LOG_INSTANCE().Create("D:\\ProjectMC\\Server\\Out\\Log\\Log"))
	//{
	//	return false;
	//}

	if(false == s2::S2Net::Create())
	{
		return false;
	}

	int32_t workerCount = 6;
	int32_t sessionCount = 100;
	int32_t dbCount = 6;

	if(false == m_workerMgr.Create(workerCount))
	{
		printf("[Eror] Worker\n");
		return false;
	}
	auto& workerList = m_workerMgr.GetWorkerList();
	for(auto worker : workerList)
	{
		MESSAGE_PROCESSOR().AddSender(MessageProcessor::MESSAGE_TYPE::USER, static_cast<int32_t>(MessageProcessor::MESSAGE_GROUP_USER::DATABASE), worker->GetIdx(), 100);
	}

	if(false == m_sessionMgr.Create(sessionCount))
	{
		printf("[Eror] Session\n");
		return false;
	}

	if(false == m_dataBaseMgr.Create(dbCount, 500))
	{
		printf("[Eror] DB\n");
		return false;
	}

	if(false == m_userProssor.Create(workerCount, dbCount, 500))
	{
		printf("[Eror] Processor\n");
		return false;
	}
		
	uint32_t ip = s2::S2Net::GetIPToLong("127.0.0.1");
	//uint32_t ip = s2::S2Net::GetIPToLong("192.168.0.199");
	uint16_t port = 36101;
	if(false == m_IOCP.Create(ip, port, &m_workerMgr, &m_sessionMgr))
	{
		printf("[Eror] IOCP\n");
		return false;
	}
	
	return true;
}

void MainFrm::OnUpdate()
{
}

void MainFrm::OnDestroy()
{
	m_IOCP.Destroy();
	m_userProssor.Destroy();
	m_dataBaseMgr.Destroy();
	m_sessionMgr.Destroy();
	m_workerMgr.Destroy();

	s2::S2Net::Destroy();
}
