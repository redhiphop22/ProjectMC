#pragma once

namespace s2 {

class S2IocpWorker
{
public:
	S2IocpWorker(int32_t idx);
	virtual ~S2IocpWorker();

	virtual bool			Create();
	virtual void			Destroy();
	virtual	void			OnUpdate();

	int32_t					GetIdx()					{	return m_idx;	}
	void					SetIOCP(HANDLE IOCP)		{	m_IOCP = IOCP;	}

protected:
	S2Thread				m_thread;
	int32_t					m_idx;
	HANDLE					m_IOCP = NULL;
};

}