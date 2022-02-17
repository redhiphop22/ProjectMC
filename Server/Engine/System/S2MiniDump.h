#pragma once

#include <DbgHelp.h>

class S2ExceptionHandler : public s2::S2Singleton<S2ExceptionHandler>
{
public:
	S2ExceptionHandler();
	virtual ~S2ExceptionHandler();

	bool					Create(const char* strDmpFileName, MINIDUMP_TYPE eOption = MiniDumpNormal);
	void					SetCreateMiniDump(PEXCEPTION_POINTERS pExceptionInfo);

private:
	MINIDUMP_TYPE m_eDumpType = MiniDumpNormal;
	std::string m_strDmpFileName = "S2BugReport.i3b";
};