#pragma once

namespace s2 {

class S2Iocp
{
public:
	S2Iocp();
	virtual ~S2Iocp();

	bool					Create(uint32_t ip, uint16_t port, S2IocpWorkerMgr* workerMgr, S2IocpSessionMgr* sessionMgr, int32_t backLog = SOMAXCONN);
	void					Destroy();

private:
	HANDLE					m_IOCP = NULL;
	SOCKET					m_listenSocket = INVALID_SOCKET;
};

}