#include "S2Engine.h"
#include "S2MiniDump.h"

#pragma comment(lib, "Dbghelp.lib")

LONG WINAPI CheckUnhandledException(PEXCEPTION_POINTERS pExceptionInfo)
{
	SetErrorMode(SEM_NOGPFAULTERRORBOX);
	S2ExceptionHandler::GetInstance().SetCreateMiniDump(pExceptionInfo);
	return 0;
}

S2ExceptionHandler::S2ExceptionHandler()
{
}

S2ExceptionHandler::~S2ExceptionHandler()
{
}

bool S2ExceptionHandler::Create(const char* strDmpFileName, MINIDUMP_TYPE eOption)
{
	if(nullptr == strDmpFileName) return false;

	m_strDmpFileName = strDmpFileName;
	m_eDumpType = eOption;

	SetUnhandledExceptionFilter(CheckUnhandledException);

	return true;
}

void S2ExceptionHandler::SetCreateMiniDump(PEXCEPTION_POINTERS pExceptionInfo)
{
	// miniDump ½ÇÇà
	MINIDUMP_EXCEPTION_INFORMATION sExceptionInfo;

	sExceptionInfo.ThreadId = GetCurrentThreadId();
	sExceptionInfo.ExceptionPointers = pExceptionInfo;
	sExceptionInfo.ClientPointers = FALSE;
	
	SYSTEMTIME st;
	GetLocalTime(&st);

	std::string strFileName;
	s2::string::Format(strFileName, "%s_%04d%02d%02d%02d%02d", m_strDmpFileName.c_str(), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);

	HANDLE hFile = ::CreateFile(strFileName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, m_eDumpType, &sExceptionInfo, NULL, NULL);

	CloseHandle(hFile);
}