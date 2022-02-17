#pragma once

namespace s2 {

class S2IocpWorkerMgr
{
protected:
	using worker_list_t = std::vector<S2IocpWorker*>;

public:
	S2IocpWorkerMgr();
	virtual ~S2IocpWorkerMgr();

	virtual bool			Create(int32_t workerCount);
	void					Destroy();

	bool					Init(HANDLE IOCP);

	worker_list_t&			GetWorkerList() {	return m_workerList;	}

protected:
	worker_list_t			m_workerList;
};

}