#pragma once

//#include "S2CriticalSection.h"

#define S2LogFile S2LogFileW

#define LOG_BUFFER_MAX_COUNTER		100
#define LOG_INIT_STRING				10
#define LOG_STRING_MAX				1024
#define RV_ADD_FAIL					0x80000001
#define LOG_START_DATE				32

enum FILE_SAVE_INTERVAL
{
	FILE_SAVE_INTERVAL_DAY = 1,
	FILE_SAVE_INTERVAL_HOUR,
};

enum S2_EXPORT_BASE S2_LOG_TYPE
{
	S2_LOG_TYPE_SUCCESS,
	S2_LOG_TYPE_ERROR,
	S2_LOG_TYPE_WARNING,
};

class S2_EXPORT_BASE S2Log : public S2Thread
{
protected:
	HANDLE					m_hFile;

	std::wstring m_fileName;
	std::wstring m_startText;

	//For File
	int32_t					m_iSaveType;
	uint32_t					m_ui32FileHour;
	uint32_t					m_ui32FileDay;

	std::deque< std::wstring> m_logText;

	//For Add Log Buffer 
	//S2CriticalSection		m_CS;

	//For Save Time
	SYSTEMTIME				m_SystemTime;
	uint32_t					m_iYear;
	uint32_t					m_iMonth;
	uint32_t					m_iDay;
	uint32_t					m_iHour;
	uint32_t					m_iMinute;
	uint32_t					m_iSecond;

	// Server Version
	uint16_t					m_ui16Ver01 = 0;
	uint16_t					m_ui16Ver02 = 0;
	uint32_t					m_ui32Ver03 = 0;
	uint32_t					m_ui32Ver04 = 0;

protected:

	void					_UpdateTime(void)
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

	virtual bool			_CreateFile(void);
	virtual bool			_Working();

	template<typename ... Args>
	void WriteLog(S2_LOG_TYPE eLogType, wchar_t* format, Args ... args)
	{
		std::wstring temp;
		S2String::Format(temp, format, args ...);
		m_logText.push_back(temp);
	}

	void _WriteLogFile(std::wstring& log);

public:

	S2Log();
	virtual ~S2Log(void);

	virtual bool			Create(std::wstring& fileName, int32_t iSaveType = FILE_SAVE_INTERVAL_HOUR);
	void					Destroy();
	void					SetVersion(uint16_t ui16Ver01, uint16_t ui16Ver02, uint32_t ui32Ver03, uint32_t ui32Ver04);
	void					OnUpdate();

public:
	virtual bool OnThreadUpdate();
};