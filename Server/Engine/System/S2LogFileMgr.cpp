#include "S2Engine.h"
#include "S2LogFileMgr.h"

namespace s2 {

S2LogFileMgr::S2LogFileMgr()
{
}

S2LogFileMgr::~S2LogFileMgr(void)
{
	Destroy();
}

bool S2LogFileMgr::Create(const _s2log_char_t* wstrFileName, FILE_SAVE_INTERVAL fileSaveType)
{
	auto threadProc = [this]() -> void {
		this->OnUpdate();
	};
	return m_thread.ThreadCreateFunc(threadProc);
}

void S2LogFileMgr::Destroy(void)
{
	m_thread.DestroyThread();

	::CloseHandle(m_hFile);
}

void S2LogFileMgr::OnUpdate()
{	
	THREAD_UPDATE_START;
	//Create File 
	_UpdateTime();
	switch (m_fileSaveType)
	{
	case FILE_SAVE_INTERVAL::DAY:
		if (m_ui32FileDay != m_iDay)
		{
			_CreateFile();
		}
		break;
	case FILE_SAVE_INTERVAL::HOUR:
	default:
		if (m_ui32FileHour != m_iHour)
		{
			_CreateFile();
		}
		break;
	}
	isWorking = _Working();

	THREAD_UPDATE_END;
}

void S2LogFileMgr::WriteLog(S2_LOG_TYPE logType, const _s2log_char_t* funcName, int32_t line, const _s2log_char_t* logString, ...)
{
}

bool S2LogFileMgr::PushThread(const S2Thread::thread_id_t& id)
{
	std::lock_guard<std::mutex> mutex(m_mutex);

	auto iter = m_threadList.find(id);
	if (iter != m_threadList.end())
		return false;

	ring_buffer_t* ringLog = new ring_buffer_t();
	if(false == ringLog->Create(LOG_BUFFER_MAX_COUNTER))
		return false;

	auto result = m_threadList.insert(std::make_pair(id, ringLog));
	if (false == result.second)
		return false;

	return true;
}

void S2LogFileMgr::SetVersion(uint16_t ui16Ver01, uint16_t ui16Ver02, uint32_t ui32Ver03, uint32_t ui32Ver04)
{
	m_ui16Ver01 = ui16Ver01;
	m_ui16Ver02 = ui16Ver02;
	m_ui32Ver03 = ui32Ver03;
	m_ui32Ver04 = ui32Ver04;
}

bool S2LogFileMgr::_Create(FILE_SAVE_INTERVAL fileSaveType)
{
	m_fileSaveType = fileSaveType;

	return true;
}

bool S2LogFileMgr::_Working()
{
	bool bWorking = false;

	for (auto& iter : m_threadList)
	{
		auto* ringBuffer = iter.second;
		int32_t size = ringBuffer->Size();
		if (0 == size)
			continue;

		for (int32_t i = 0; i < size; ++i)
		{
			ThreadLogList log;
			if (false == ringBuffer->PopData(&log))
				continue;

			m_logList.push_back(log);
		}
	}

	while (false == m_logList.empty())
	{
		bWorking = true;
		ThreadLogList log = m_logList.front();
		_WriteFile(log.m_log, log.m_size);
		m_logList.pop_front();
	}

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
}

void S2LogFileMgr::_WriteFile(const _s2log_char_t* strLog, int32_t size)
{
}

S2LogFileMgr::ring_buffer_t* S2LogFileMgr::_GetThreadList()
{
	std::thread::id threadId = std::this_thread::get_id();

	auto iter = m_threadList.find(threadId);
	if(iter == m_threadList.end())
		return nullptr;

	return iter->second;
}
}