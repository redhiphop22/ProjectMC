#pragma once

//#include "S2CriticalSection.h"

class S2Thread;

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

class S2LogFileMgr : public S2MessageReceiver
{
public:
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

	virtual bool			Create(const _s2log_char_t* wstrFileName);
	virtual void			Destroy();	
	void					OnUpdate();

	virtual void			WriteLog(S2_LOG_TYPE logType, const _s2log_char_t* funcName, int32_t line, const _s2log_char_t* logString, ...);

	bool					PushThread(const std::thread::id& id);
	void					SetVersion(uint16_t ver01, uint16_t ver02, uint32_t ver03, uint32_t ver04);
	void					GetNowTime(tm& now);

protected:

	virtual bool			_Working();

	virtual bool			_CreateFile();
	virtual void			_WriteLog(const _s2log_char_t* strLog, int32_t size);
	virtual void			_WriteFile(const _s2log_char_t* strLog, int32_t size);

	ring_buffer_t*			_GetThreadList();

	virtual bool			OnMessageUpdate(int32_t groupIdx, void* data);

protected:
	const uint32_t			m_fileMaxSize = 2097152;	// 2MB

	S2Thread				m_thread;

	HANDLE					m_file = NULL;

	int32_t					m_fileSize = 0;
	//For File
	uint32_t				m_recordDay;	

	// Server Version
	uint16_t				m_ver01 = 0;
	uint16_t				m_ver02 = 0;
	uint32_t				m_ver03 = 0;
	uint32_t				m_ver04 = 0;

	std::mutex				m_mutex;
	S2MessageProcessor<ThreadLogList>	m_message;
};
}