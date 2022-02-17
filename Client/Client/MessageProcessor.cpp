#include "pch.h"
#include "MessageProcessor.h"

void threadProc(MessageThread* thread)
{
	thread->OnThreadExecute();
}

MessageThread::MessageThread(ControllerBase* controller)
	: m_controller(controller)
{
}

MessageThread::~MessageThread()
{
}

bool MessageThread::Create(bool useThread)
{
	m_useThread = useThread;

	if(m_useThread)
	{
		m_thread = ::CreateThread(NULL, 4096, (LPTHREAD_START_ROUTINE)threadProc, this, 0, &m_threadID);
		if(NULL == m_thread)
			return false;

		if(FALSE == SetThreadPriority(m_thread, THREAD_PRIORITY_NORMAL))
			return false;
	}

	return true;
}

void MessageThread::Destroy()
{
	if(m_isRunning)
	{
		m_isRunning = false;
		::WaitForSingleObject(m_thread, 0xFFFFFFFF);
	}
	::CloseHandle(m_thread);
	m_thread = NULL;
}

void MessageThread::PushData(MessageInfo& message)
{
	m_message = message;

	m_state = THREAD_STATE::RUNNING;

	if(false == m_useThread)
	{
		OnRunning();
		m_state = THREAD_STATE::EXIT;
	}
}

void MessageThread::OnThreadExecute()
{
	m_isRunning = true;
	while(m_isRunning)
	{
		if(THREAD_STATE::RUNNING == m_state)
		{
			OnRunning();
			m_state = THREAD_STATE::EXIT;
		}
		else
		{
			::WaitForSingleObject(m_thread, 1);
		}
	}
}

void MessageThread::OnRunning()
{
	m_controller->PacketParsing(m_message.m_user, m_message.m_protocol, m_message.m_buffer, m_message.m_size);
}

MessageProcessor::MessageProcessor(ControllerBase* controller)
	: m_controller(controller)
{
}

MessageProcessor::~MessageProcessor()
{
	Destroy();
}

bool MessageProcessor::Create(int32_t workerCount, bool useThread)
{
	for(int32_t i = 0 ; i < workerCount ; ++i)
	{
		MessageThread* thread = new MessageThread(m_controller);
		if(nullptr == thread)
			return false;
		if(false == thread->Create(useThread))
			return false;
		m_threadList.push_back(thread);
	}
	return true;
}

void MessageProcessor::Destroy()
{
	for(auto& thread : m_threadList)
	{
		thread->Destroy();
		delete thread;
		thread = nullptr;
	}
	m_threadList.clear();
}

void MessageProcessor::PushData(User* user, packet_protocol_t protocol, char* buffer, int32_t size)
{
	MessageThread::MessageInfo message;
	message.Set(user, protocol, buffer, size);
	m_packetList.emplace_back(std::move(message));
}

void MessageProcessor::OnUpdate()
{
	while(0 != m_packetList.size())
	{
		auto& message = m_packetList.front();

		auto thread = GetWaitThread();
		if(nullptr == thread)
			return;

		thread->PushData(message);
		m_packetList.pop_front();
	}
}

MessageThread* MessageProcessor::GetWaitThread() 
{
	for(auto thread : m_threadList)
	{
		if(MessageThread::THREAD_STATE::EXIT == thread->GetState())
			thread->SetState(MessageThread::THREAD_STATE::WATTING);

		if(MessageThread::THREAD_STATE::WATTING != thread->GetState())
			continue;

		return thread;
	}
	return nullptr;
}