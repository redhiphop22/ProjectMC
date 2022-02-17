#pragma once

class DataBaseWorker;

class DataBaseMgr : public s2::S2MessageReceiver
{
public:
	DataBaseMgr();
	~DataBaseMgr();

	bool					Create(int32_t workerCount);
	void					Destroy();

	void					OnUpdate();
	virtual bool			OnMessageUpdate(int32_t groupIdx, void* data) override;

	DataBaseWorker*			PopWaitingThread();
	bool					PushWaitingThread(DataBaseWorker* thread);

private:
	s2::S2Thread			m_thread;

	std::vector<DataBaseWorker*> m_workerList;
	std::stack<DataBaseWorker*> m_waitingList;
};