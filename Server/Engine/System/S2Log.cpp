#include "stdafx.h"
#include "S2Log.h"

S2Log::S2Log()
{
	m_hFile = NULL;

	m_ui16Ver01 = 0;
	m_ui16Ver02 = 0;
	m_ui32Ver03 = 0;
	m_ui32Ver04 = 0;
}

S2Log::~S2Log(void)
{
	Destroy();
}

bool S2Log::Create(std::wstring& fileName, int32_t iSaveType)
{
	//Set init Value
	//S2Memory::FillZero(m_ppLogBuffer, sizeof(m_ppLogBuffer));

	m_fileName = fileName;
	m_iSaveType = iSaveType;
	//m_iBufferCount = iLogBufferCount;

	_UpdateTime();

	wchar_t wstrData[MAX_PATH];
	MultiByteToWideChar(CP_ACP, 0, __DATE__, -1, wstrData, MAX_PATH);

	wchar_t wstrTime[MAX_PATH];
	MultiByteToWideChar(CP_ACP, 0, __TIME__, -1, wstrTime, MAX_PATH);

	S2String::Format(m_startText, L"-- COMPILE DATE : Ver:%d.%d.%d.%d, BuildDate:%s %s, StartDate:%04d-%02d-%02d %02d:%02d --",
		m_ui16Ver01, m_ui16Ver02, m_ui32Ver03, m_ui32Ver04, wstrData, wstrTime, m_iYear, m_iMonth, m_iDay, m_iHour, m_iMinute);

	//Create File 
	if(false == _CreateFile())
		return false;

	if(false == S2Thread::Create())
		return false;

	return true;
}

void S2Log::Destroy(void)
{
	S2Thread::Destroy();

	::CloseHandle(m_hFile);
}

void S2Log::SetVersion(UINT16 ui16Ver01, UINT16 ui16Ver02, UINT32 ui32Ver03, UINT32 ui32Ver04)
{
	m_ui16Ver01 = ui16Ver01;
	m_ui16Ver02 = ui16Ver02;
	m_ui32Ver03 = ui32Ver03;
	m_ui32Ver04 = ui32Ver04;
}

bool S2Log::_CreateFile()
{
	std::wstring fileName;
	S2String::Format(fileName, L"%s_%02d_%02d_%02d_%02d.txt", m_fileName, m_iYear, m_iMonth, m_iDay, m_iHour);

	//Close Old File 
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}

	// Create New File
	m_hFile = ::CreateFileW(fileName.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (m_hFile == INVALID_HANDLE_VALUE)
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

		_WriteLogFile(m_startText);
	}

	return true;
}

bool S2Log::_Working()
{
	return true;

	//INT32 iLogCount;
	//LOG_FILE_BUFFER* pLogBuffer;
	//BOOL bWorking = FALSE;

	////Write Log 
	//for (UINT32 j = 0; j < m_ui32WorkCounter; j++)
	//{
	//	iLogCount = m_ppLogBuffer[j]->GetBufferCount();
	//	for (INT32 i = 0; i < iLogCount; i++)
	//	{
	//		pLogBuffer = (LOG_FILE_BUFFER*)m_ppLogBuffer[j]->Pop();
	//		_WriteLogFile(pLogBuffer->_strMessage);
	//		m_ppLogBuffer[j]->PopIdx();
	//		bWorking = TRUE;
	//	}
	//}
	//return bWorking;
}

void S2Log::_WriteLogFile(std::wstring& log)
{
	INT32 i32Length;
	::WriteFile(m_hFile, log.c_str(), log.size(), (LPDWORD)&i32Length, NULL);
	//S2TRACE(wstrLog);
}

bool S2Log::OnThreadUpdate()
{
	//Create File 
	_UpdateTime();
	switch (m_iSaveType)
	{
	case FILE_SAVE_INTERVAL_DAY:
		if (m_ui32FileDay != m_iDay)	
		{
			_CreateFile();
		}
		break;
	case FILE_SAVE_INTERVAL_HOUR:
	default:
		if (m_ui32FileHour != m_iHour)
		{
			_CreateFile();
		}
		break;
	}

	return _Working();
}