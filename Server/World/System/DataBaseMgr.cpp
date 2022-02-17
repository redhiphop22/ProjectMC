#include "World.h"
#include "DataBaseMgr.h"
#include "DataBaseWorker.h"

DataBaseMgr::DataBaseMgr()
{
}

DataBaseMgr::~DataBaseMgr()
{
}

bool DataBaseMgr::Create(int32_t workerCount)
{	
	if(false == MESSAGE_PROCESSOR().SetReceiver(MessageProcessor::MESSAGE_TYPE::DATABASE, this))
	{
		return false;
	}

	for(int32_t i = 0 ; i < workerCount ; ++i)
	{
		DataBaseWorker* worker = new DataBaseWorker(i);
		if(nullptr == worker)
		{
			return false;
		}
		if(false == worker->Create("127.0.0.1", 33006, "server", "Cryptect12!", "metacampus"))
		{
			return false;
		}
		m_workerList.push_back(worker);
		m_waitingList.push(worker);
	}
	
	auto threadProc = [this]() -> void {
		this->OnUpdate();
	};
	return m_thread.ThreadCreateFunc(threadProc);
}

void DataBaseMgr::Destroy()
{
	m_thread.DestroyThread();
	for(auto& iter : m_workerList)
	{
		iter->Destroy();
		delete iter;
		iter = nullptr;
	}
	m_workerList.clear();
}

void DataBaseMgr::OnUpdate()
{
	THREAD_UPDATE_START;

	for(auto& thread : m_workerList)
	{
		DataBaseWorker* worker = (DataBaseWorker*)thread;

		if(DataBaseWorker::WORKER_STATE::FINISH == worker->GetState())
		{
			worker->SetState(DataBaseWorker::WORKER_STATE::INIT);
			PushWaitingThread(worker);
		}
	}

	MESSAGE_PROCESSOR().OnUpdate(MessageProcessor::MESSAGE_TYPE::DATABASE);

	THREAD_UPDATE_END;
}

bool DataBaseMgr::OnMessageUpdate(int32_t groupIdx, void* data)
{
	DataBaseWorker* thread = (DataBaseWorker*)PopWaitingThread();
	if(nullptr == thread)
		return false;

	thread->PushQuery((char*)data);

	return true;
}

DataBaseWorker* DataBaseMgr::PopWaitingThread()
{
	if(m_waitingList.empty())
		return nullptr;

	auto& thread = m_waitingList.top();
	m_waitingList.pop();
	return thread;
}

bool DataBaseMgr::PushWaitingThread(DataBaseWorker* thread)
{
	m_waitingList.push(thread);

	return true;
}
