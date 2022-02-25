#pragma once

namespace s2 {

class S2LogFile;

class S2LogAccessor : public S2Singleton<S2LogAccessor>
{
public:
	S2LogAccessor() = default;
	~S2LogAccessor() = default;

	void					SetLog(S2LogFile* log)	{	m_log = log;	}
	S2LogFile*				GetLog()				{	return m_log;	}

private:
	S2LogFile*				m_log = nullptr;
};

}

#define __FUNCTIONNAME__ (strrchr(__FUNCTION__, '::') ? strrchr(__FUNCTION__, '::') + 1 : __FUNCTION__)

#define S2LOG_INIT(log)			s2::S2LogAccessor::GetInstance().SetLog(log)
#define S2LOG_INSTANCE			s2::S2LogAccessor::GetInstance().GetLog()
#define S2LOG_ERROR(fmt, ...)	s2::S2LogAccessor::GetInstance().GetLog()->WriteLog(s2::S2LogFileMgr::S2_LOG_TYPE::TYPE_ERROR, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__);
#define S2LOG_WARNING(fmt, ...)	s2::S2LogAccessor::GetInstance().GetLog()->WriteLog(s2::S2LogFileMgr::S2_LOG_TYPE::TYPE_WARNING, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__);
#define S2LOG_INFO(fmt, ...)	s2::S2LogAccessor::GetInstance().GetLog()->WriteLog(s2::S2LogFileMgr::S2_LOG_TYPE::TYPE_INFO, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__);
#define S2LOG(fmt, ...)			s2::S2LogAccessor::GetInstance().GetLog()->WriteLog(s2::S2LogFileMgr::S2_LOG_TYPE::TYPE_INFO, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__);