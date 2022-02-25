#include "S2Engine.h"
#include "S2LogFileMgr.h"
#include "S2Thread.h"

namespace s2 {

S2LogFileMgr::S2LogFileMgr()
{
}

S2LogFileMgr::~S2LogFileMgr(void)
{
}

bool S2LogFileMgr::Create(const _s2log_char_t* wstrFileName)
{
	//Create File 
	if (false == _CreateFile())
	{
		return false;
	}

	m_message.SetReceiver(this);

	auto threadProc = [this]() -> void {
		this->OnUpdate();
	};
	return m_thread.CreateThread(threadProc, THREAD_PRIORITY_NORMAL, __FILE__, false);
}

void S2LogFileMgr::Destroy(void)
{
	m_thread.DestroyThread();

	if(NULL != m_file)
	{
		::CloseHandle(m_file);
		m_file = NULL;
	}
}

void S2LogFileMgr::OnUpdate()
{	
	THREAD_UPDATE_START;

	tm tmNow;
	GetNowTime(tmNow);
		
	if(m_recordDay != tmNow.tm_mday ||
		m_fileMaxSize < m_fileSize)
	{
		_CreateFile();
	}

	isWorking = m_message.MessageUpdate();

	THREAD_UPDATE_END;
}

void S2LogFileMgr::WriteLog(S2_LOG_TYPE logType, const _s2log_char_t* funcName, int32_t line, const _s2log_char_t* logString, ...)
{
}

bool S2LogFileMgr::PushThread(const std::thread::id& id)
{
	//std::lock_guard<std::mutex> mutex(m_mutex);
	uint32_t processorId;
	memcpy(&processorId, &id, sizeof(uint32_t));
	if(false == m_message.AddSender(0, processorId, 100))
	{
		return false;
	}
	
	return true;
}

bool S2LogFileMgr::OnMessageUpdate(int32_t groupIdx, void* data)
{
	ThreadLogList* log = (ThreadLogList*)data;
	
	_WriteFile(log->m_log, log->m_size);

	return true;
}

void S2LogFileMgr::SetVersion(uint16_t ver01, uint16_t ver02, uint32_t ver03, uint32_t ver04)
{
	m_ver01 = ver01;
	m_ver02 = ver02;
	m_ver03 = ver03;
	m_ver04 = ver04;
}

void S2LogFileMgr::GetNowTime(tm& now)
{
	time_t nowTime = S2Time::Now();
	localtime_s(&now, &nowTime);
}

bool S2LogFileMgr::_Working()
{
	bool bWorking = false;

	

	return bWorking;
}

bool S2LogFileMgr::_CreateFile()
{
	return TRUE;
}

void S2LogFileMgr::_WriteLog(const _s2log_char_t* strLog, int32_t size)
{
	ring_buffer_t* threadLog = _GetThreadList();
	if(nullptr == threadLog)
		return;

	auto logList = threadLog->GetPushData();
	logList->m_size = size;
	memcpy(logList->m_log, strLog, size);
	threadLog->PushCompleted();
}

void S2LogFileMgr::_WriteFile(const _s2log_char_t* strLog, int32_t size)
{
}

S2LogFileMgr::ring_buffer_t* S2LogFileMgr::_GetThreadList()
{
	std::thread::id threadId = std::this_thread::get_id();

	uint32_t processorId;
	memcpy(&processorId, &threadId, sizeof(uint32_t));
	auto ringBuffer = m_message.GetRingBuffer(0, processorId);
	return ringBuffer;
}
}
