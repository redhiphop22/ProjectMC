#pragma once

class MainFrm
{
public:
	MainFrm() = default;
	~MainFrm() = default;

	bool					OnInit();
	void					OnUpdate();
	void					OnDestroy();

private:
	bool					m_isRunning = false;

	s2::S2IocpWorkerMgr		m_workerMgr;
	IocpSessionMgr			m_sessionMgr;
	s2::S2Iocp				m_IOCP;

	UserProcessor			m_userProssor;
	DataBaseMgr				m_dataBaseMgr;
};

extern MainFrm* mainFrm;