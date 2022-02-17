#include "S2Engine.h"
#include "S2LogFileA.h"

namespace s2 {

S2LogFileA::S2LogFileA()
{
}

S2LogFileA::~S2LogFileA()
{
	Destroy();
}

bool S2LogFileA::Create(const char* wstrFileName, FILE_SAVE_INTERVAL fileSaveType)
{
	m_strFileName = wstrFileName;

	_UpdateTime();

	s2::string::Format(m_strStartText, "-- COMPILE DATE : Ver:%d.%d.%d.%d, BuildDate:%s %s, StartDate:%04d-%02d-%02d %02d:%02d --\r\n",
		m_ui16Ver01, m_ui16Ver02, m_ui32Ver03, m_ui32Ver04, __DATE__, __TIME__, m_iYear, m_iMonth, m_iDay, m_iHour, m_iMinute);

	//Create File 
	if (false == _CreateFile())
	{
		return false;
	}

	if(false == S2LogFileMgr::Create(wstrFileName, fileSaveType))
	{
		return false;
	}

	return true;
}

void S2LogFileA::Destroy()
{
	S2LogFileMgr::Destroy();
}

void S2LogFileA::WriteLog(S2_LOG_TYPE logType, const char* funcName, int32_t line, const char* logString, ...)
{
	auto getErrorType = [](S2_LOG_TYPE logType) -> const char* {
		switch(logType)
		{
		case S2_LOG_TYPE::TYPE_ERROR:
			return "ERROR";
		case S2_LOG_TYPE::TYPE_WARNING:
			return "WARNG";
		case S2_LOG_TYPE::TYPE_INFO:
			return "INFO_";
		}
		return "";
	};

	char strTemp[LOG_STRING_MAX];
	va_list marker;
	va_start(marker, logString);
	_vsnprintf_s(strTemp, LOG_STRING_MAX, logString, marker);
	va_end(marker);

	char strLog[LOG_STRING_MAX];
	int32_t insertSize = _snprintf_s(strLog, _countof(strLog), LOG_STRING_MAX, "[%s][%s(%d)] %s\r\n", getErrorType(logType), funcName, line, strTemp);

	_WriteLog(strLog, insertSize);
}

bool S2LogFileA::_CreateFile(void)
{
	std::string strFileName;
	s2::string::Format(strFileName, "%s_%02d_%02d_%02d_%02d.txt", m_strFileName.c_str(), m_iYear, m_iMonth, m_iDay, m_iHour);

	//Close Old File 
	if(INVALID_HANDLE_VALUE != m_hFile)
	{
		::CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}

	// Create New File
	m_hFile = ::CreateFileA(strFileName.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if(INVALID_HANDLE_VALUE == m_hFile)
	{
		::CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
		return false;
	}
	else
	{
		SetFilePointer(m_hFile, 0, NULL, FILE_END);

		m_ui32FileHour = m_iHour;
		m_ui32FileDay = m_iDay;

		_WriteFile(m_strStartText.c_str(), m_strStartText.size());
	}

	return true;
}

void S2LogFileA::_WriteFile(const char* strLog, int32_t size)
{
	int32_t length;
	::WriteFile(m_hFile, strLog, size, (LPDWORD)&length, NULL);
	S2TRACE(strLog);
}

}