#include "Zone.h"
#include "MainFrm.h"

MainFrm* mainFrm = nullptr;

bool MainFrm::OnInit()
{
	if(false == CONFIG.Create("..\\Config\\zone.conf"))
	{
		printf("[Eror] Config\n");
		return false;
	}

	if(false == s2::S2Net::Create())
	{
		return false;
	}

	int32_t icopWorkerCount = 6;
	int32_t icopSessionCount = 100;
	int32_t dbCount = 6;

	if(false == m_workerMgr.Create(icopWorkerCount))
	{
		printf("[Eror] Worker\n");
		return false;
	}

	if(false == m_sessionMgr.Create(icopSessionCount))
	{
		printf("[Eror] Session\n");
		return false;
	}

	//if(false == m_dataBaseMgr.Create(dbCount, 500))
	//{
	//	printf("[Eror] DB\n");
	//	return false;
	//}

	if(false == m_userProssor.Create())
	{
		printf("[Eror] Processor\n");
		return false;
	}
		
	uint32_t ip = s2::S2Net::GetIPToLong(CONFIG.GetJson()["serverIP"].GetString());
	uint16_t port = CONFIG.GetJson()["serverPort"].GetUint();
	if(false == m_IOCP.Create(ip, port, &m_workerMgr, &m_sessionMgr))
	{
		printf("[Eror] IOCP\n");
		return false;
	}
	
	MESSAGE_PROCESSOR().AddSender(MessageProcessor::MESSAGE_TYPE::USER, static_cast<int32_t>(MessageProcessor::MESSAGE_GROUP_USER::IOCP), icopWorkerCount, 100);

	return true;
}

void MainFrm::OnUpdate()
{
}

void MainFrm::OnDestroy()
{
	m_IOCP.Destroy();
	//m_userProssor.Destroy();
	//m_dataBaseMgr.Destroy();
	m_sessionMgr.Destroy();
	m_workerMgr.Destroy();

	s2::S2Net::Destroy();
}
