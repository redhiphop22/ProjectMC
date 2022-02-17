#include "S2Engine.h"
#include "S2Server.h"

//S2LogFile* g_pLog = NULL;
namespace s2 {

S2Server::S2Server()
{
	m_eServerState = S2_SERVER_STATE::NONE;
}

S2Server::~S2Server()
{
}

S2_RESULT_CODE S2Server::StartServer(const char* dumpFile, INT32 i32InitDelayTimeSec)
{
	//if(false == S2LOG_INSTANCE().Create("C:\\Project\\Server\\Out\\Log\\Log"))
	//{
	//	return S2_RESULT_ERROR;
	//}

	//S2MiniDump::SetExceptionHandler(dumpFile);

	if (false == S2Time::Init())
		return S2_RESULT_ERROR_ST_TIMER;

	//g_pLog = new S2LogFile;
	//if (NULL == g_pLog)
	//	return S2_RESULT_ERROR_ST_LOG;
	//if (FALSE == g_pLog->Create(m_pConfig->m_strLogFilePath, FILE_SAVE_INTERVAL_HOUR, m_pConfig->m_i32LogBufferCount))
	//	return S2_RESULT_ERROR_ST_LOG;

	//Network Init
	if (false == S2Net::Create())
		return S2_RESULT_ERROR_ST_NETWORK;

	m_eServerState = S2_SERVER_STATE::LOADING;
	//m_dt32DelayTime = 0;//S2Time::StandTime();
	m_dt32DelayTime.AddTime(S2_DATE_ADD_TYPE::SECOND, i32InitDelayTimeSec);

	auto threadProc = [this]() -> void {
		this->OnThreadUpdate();
	};
	if(false == m_thread.ThreadCreateFunc(threadProc))
		return S2_RESULT_ERROR_ST_THREAD;

	return S2_RESULT_SUCCESS;
}

void S2Server::SetInit()
{
	m_eServerState = S2_SERVER_STATE::INIT;
}

void S2Server::OnThreadUpdate()
{
	THREAD_UPDATE_START;

	switch (m_eServerState)
	{
	case S2_SERVER_STATE::LOADING:
		{
			m_eServerState = S2_SERVER_STATE::INIT;
			// 10초 이상이면 문제
			//if (S2Time::StandTime() > m_dt32DelayTime)
			{
			//	StopServer();
			}
		}
		break;
	case S2_SERVER_STATE::INIT:
		{
			if (S2_RESULT_SUCCESS != OnInit())
			{
				StopServer();
			}
		}
		break;
	case S2_SERVER_STATE::START:
		{
			OnUpdate();
		}
		break;
	case S2_SERVER_STATE::END:
		{
			OnDestroy();
		}
		break;
	}

	THREAD_UPDATE_END;
}

S2_RESULT_CODE S2Server::OnInit()
{
	S2LOG_ERROR("Error Test");

	m_eServerState = S2_SERVER_STATE::START;

	return S2_RESULT_SUCCESS;
}

void S2Server::OnUpdate()
{
}

void S2Server::OnDestroy()
{
	m_thread.DestroyThread();

	exit(0);
}

void S2Server::StopServer()
{
	m_eServerState = S2_SERVER_STATE::END;
}
}