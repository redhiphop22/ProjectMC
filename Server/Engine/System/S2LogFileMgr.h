#pragma once

//#include "S2CriticalSection.h"

#ifdef USE_FILE_UNICODE
#define S2LogFile S2LogFileW
#define _s2log_char_t wchar_t
#else
#define S2LogFile S2LogFileA
#define _s2log_char_t char
#endif

#define LOG_BUFFER_MAX_COUNTER		100
#define LOG_INIT_STRING				10
#define LOG_STRING_MAX				1024
#define RV_ADD_FAIL					0x80000001
#define LOG_START_DATE				32

namespace s2 {

class S2LogFileMgr : public S2Thread
{
public:
	enum class FILE_SAVE_INTERVAL
	{
		DAY = 1,
		HOUR,
	};

	enum class S2_LOG_TYPE : int32_t
	{
		TYPE_ERROR,
		TYPE_WARNING,
		TYPE_INFO,
	};
	
protected:
	struct ThreadLogList
	{
		int16_t			m_size;
		_s2log_char_t	m_log[LOG_STRING_MAX];

		ThreadLogList() {}
		ThreadLogList(const _s2log_char_t* log, int32_t size)
		{
			m_size = size;
			s2::string::strcpy(m_log, LOG_STRING_MAX, log);
		}
	};

	using ring_buffer_t = S2RingBuffer<ThreadLogList>;

public:

	S2LogFileMgr();
	virtual ~S2LogFileMgr();

	virtual bool			Create(const _s2log_char_t* wstrFileName, FILE_SAVE_INTERVAL fileSaveType = FILE_SAVE_INTERVAL::HOUR);
	virtual void			Destroy();	
	virtual void			WriteLog(S2_LOG_TYPE logType, const _s2log_char_t* funcName, int32_t line, const _s2log_char_t* logString, ...);

	bool					PushThread(const S2Thread::thread_id_t& id);
	void					SetVersion(uint16_t ui16Ver01, uint16_t ui16Ver02, uint32_t ui32Ver03, uint32_t ui32Ver04);

	virtual bool			OnThreadUpdate() override;

protected:

	bool					_Create(FILE_SAVE_INTERVAL iSaveType);
	void					_UpdateTime()
	{
		::GetLocalTime(&m_SystemTime);
		m_iYear = m_SystemTime.wYear;
		m_iMonth = m_SystemTime.wMonth;
		m_iDay = m_SystemTime.wDay;
		m_iHour = m_SystemTime.wHour;
		m_iMinute = m_SystemTime.wMinute;
		m_iSecond = m_SystemTime.wSecond;
		return;
	}

	virtual bool			_Working();

	virtual bool			_CreateFile();
	virtual void			_WriteLog(const _s2log_char_t* strLog, int32_t size);
	virtual void			_WriteFile(const _s2log_char_t* strLog, int32_t size);

	ring_buffer_t*			_GetThreadList();

protected:

	HANDLE m_hFile = NULL;

	//For File
	FILE_SAVE_INTERVAL m_fileSaveType;
	uint32_t m_ui32FileHour;
	uint32_t m_ui32FileDay;

	//For Add Log Buffer 
	//S2CriticalSection		m_CS;

	//For Save Time
	SYSTEMTIME m_SystemTime;
	uint32_t m_iYear;
	uint32_t m_iMonth;
	uint32_t m_iDay;
	uint32_t m_iHour;
	uint32_t m_iMinute;
	uint32_t m_iSecond;

	// Server Version
	uint16_t m_ui16Ver01 = 0;
	uint16_t m_ui16Ver02 = 0;
	uint32_t m_ui32Ver03 = 0;
	uint32_t m_ui32Ver04 = 0;

	std::mutex m_mutex;
	std::unordered_map<s2::S2Thread::thread_id_t, ring_buffer_t*> m_threadList;
	std::deque<ThreadLogList> m_logList;
};
}
#define __FUNCTIONNAME__ (strrchr(__FUNCTION__, '::') ? strrchr(__FUNCTION__, '::') + 1 : __FUNCTION__)

#define S2LOG_INSTANCE()		s2::S2LogFile::GetInstance()
#define S2LOG_ERROR(fmt, ...)	s2::S2LogFile::GetInstance().WriteLog(s2::S2LogFileMgr::S2_LOG_TYPE::TYPE_ERROR, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__);
#define S2LOG_WARNING(fmt, ...)	s2::S2LogFile::GetInstance().WriteLog(s2::S2LogFileMgr::S2_LOG_TYPE::TYPE_WARNING, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__);
#define S2LOG_INFO(fmt, ...)	s2::S2LogFile::GetInstance().WriteLog(s2::S2LogFileMgr::S2_LOG_TYPE::TYPE_INFO, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__);
#define S2LOG(fmt, ...)			s2::S2LogFile::GetInstance().WriteLog(s2::S2LogFileMgr::S2_LOG_TYPE::TYPE_INFO, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__);