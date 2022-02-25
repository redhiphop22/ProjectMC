#include "S2Engine.h"
#include "S2LogFileA.h"
#include <time.h>

namespace s2 {

S2LogFileA::S2LogFileA()
{
}

S2LogFileA::~S2LogFileA()
{
	Destroy();
}

bool S2LogFileA::Create(const char* strFileName)
{
	m_fileName = strFileName;

	time_t now = S2Time::Now();
	tm tmNow;
	localtime_s(&tmNow, &now);

	s2::string::Format(m_startText, "-- COMPILE DATE : Ver:%d.%d.%d.%d, BuildDate:%s %s, StartDate:%04d-%02d %02d:%02d:%02d --\r\n",
		m_ver01, m_ver02, m_ver03, m_ver04, __DATE__, __TIME__, tmNow.tm_mon+1, tmNow.tm_mday, tmNow.tm_hour, tmNow.tm_min, tmNow.tm_sec);

	if(false == S2LogFileMgr::Create(strFileName))
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
			return "EROR";
		case S2_LOG_TYPE::TYPE_WARNING:
			return "WARN";
		case S2_LOG_TYPE::TYPE_INFO:
			return "INFO";
		}
		return "";
	};

	char strTemp[LOG_STRING_MAX];
	va_list marker;
	va_start(marker, logString);
	_vsnprintf_s(strTemp, LOG_STRING_MAX, logString, marker);
	va_end(marker);

	tm tmNow;
	GetNowTime(tmNow);

	char log[LOG_STRING_MAX];
	int32_t insertSize = _snprintf_s(log, _countof(log), LOG_STRING_MAX, "[%02d.%02d %02d:%02d:%02d][%s][%s(%d)] %s", 
		tmNow.tm_mon+1, tmNow.tm_mday, tmNow.tm_hour, tmNow.tm_min, tmNow.tm_sec,
		getErrorType(logType), funcName, line, strTemp);

	_WriteLog(log, insertSize);
}

bool S2LogFileA::_CreateFile(void)
{
	tm tmNow;
	GetNowTime(tmNow);

	std::string fileName;
	s2::string::Format(fileName, "%s_%04d%02d%02d_%02d%02d%02d.txt", m_fileName.c_str(), tmNow.tm_year + 1900, tmNow.tm_mon+1, tmNow.tm_mday, tmNow.tm_hour, tmNow.tm_min, tmNow.tm_sec);

	//Close Old File 
	if(INVALID_HANDLE_VALUE != m_file)
	{
		::CloseHandle(m_file);
		m_file = INVALID_HANDLE_VALUE;
	}

	// Create New File
	m_file = ::CreateFileA(fileName.c_str(), FILE_SHARE_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(INVALID_HANDLE_VALUE == m_file)
	{
		::CloseHandle(m_file);
		m_file = INVALID_HANDLE_VALUE;
		return false;
	}

	m_fileSize = ::GetFileSize(m_file, nullptr);

	::SetFilePointer(m_file, 0, NULL, FILE_END);

	m_recordDay = tmNow.tm_mday;

	_WriteFile(m_startText.c_str(), m_startText.size());

	return true;
}

void S2LogFileA::_WriteFile(const char* strLog, int32_t size)
{
	int32_t length;
	::WriteFile(m_file, strLog, size-1, (LPDWORD)&length, NULL);
	S2TRACE(strLog);
	m_fileSize += length;
}

}