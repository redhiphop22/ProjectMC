#include "World.h"
#include "DataBaseMgr.h"
#include "DataBaseWorker.h"

DataBaseMgr::DataBaseMgr()
{
}

DataBaseMgr::~DataBaseMgr()
{
}

bool DataBaseMgr::Create(int32_t threadCount, int32_t bufferCount)
{	
	if(false == MESSAGE_PROCESSOR().SetReceiver(MessageProcessor::MESSAGE_TYPE::DATABASE, this))
	{
		return false;
	}

	for(int32_t i = 0 ; i < threadCount ; ++i)
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
		m_threadList.push_back(worker);
	}
	MakeWaitingList();
	
	return ThreadCreate();
}

void DataBaseMgr::Destroy()
{
	S2Thread::Destroy();
	S2ThreadPool::Destroy();
}

bool DataBaseMgr::OnThreadUpdate()
{
	for(auto& thread : m_threadList)
	{
		DataBaseWorker* worker = (DataBaseWorker*)thread;

		if(DataBaseWorker::WORKER_STATE::FINISH == worker->GetState())
		{
			worker->SetState(DataBaseWorker::WORKER_STATE::INIT);
			PushWaitingThread(worker);
		}
	}

	MESSAGE_PROCESSOR().OnUpdate(MessageProcessor::MESSAGE_TYPE::DATABASE);

	return true;
}

bool DataBaseMgr::OnMessageUpdate(int32_t groupIdx, void* data)
{
	char* buffer = (char*)data;

	DataBaseWorker* thread = (DataBaseWorker*)PopWaitingThread();
	if(nullptr == thread)
		return false;

	thread->PushQuery(buffer);

	return true;
}