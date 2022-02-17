#pragma once

#include "S2LogFileMgr.h"

namespace s2 {

class S2LogFileA : public s2::S2LogFileMgr, public S2Singleton<S2LogFileA>
{
public:
	S2LogFileA();
	virtual ~S2LogFileA();

	virtual bool			Create(const char* wstrFileName, FILE_SAVE_INTERVAL fileSaveType = FILE_SAVE_INTERVAL::HOUR) override;
	virtual void			Destroy() override;
	virtual void			WriteLog(S2_LOG_TYPE logType, const char* funcName, int32_t line, const char* logString, ...) override;

protected:
	virtual bool			_CreateFile() override;
	virtual void			_WriteFile(const char* strLog, int32_t size) override;

private:
	std::string				m_strFileName;
	std::string				m_strStartText;
};
}
