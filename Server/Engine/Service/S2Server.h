#pragma once

enum S2_RESULT_CODE
{
	S2_RESULT_SUCCESS,

	S2_RESULT_ERROR = 0x80000001,
	S2_RESULT_ERROR_ST_TIMER,
	S2_RESULT_ERROR_ST_NETWORK,
	S2_RESULT_ERROR_ST_LOG,
	S2_RESULT_ERROR_ST_THREAD,
	S2_RESULT_ERROR_ST_CONFIG,
	S2_RESULT_ERROR_ST_MAINFRM,
};

namespace s2 {

class S2Server : public S2Thread
{
	enum class S2_SERVER_STATE : int32_t
	{
		NONE = 0,
		LOADING,
		INIT,
		START,
		END,
	};

public:
	S2Server();
	virtual ~S2Server();

	virtual S2_RESULT_CODE	StartServer(const char* dumpFile, INT32 i32InitDelayTimeSec = 10);
	void					SetInit();
	void					StopServer();

protected:
	virtual S2_RESULT_CODE	OnInit();
	virtual void			OnUpdate();
	virtual void			OnDestroy();

	virtual bool			OnThreadUpdate() override;

protected:
	//S2Config* m_pConfig;

private:
	S2_SERVER_STATE			m_eServerState;
	date32_t				m_dt32DelayTime;
};
}
//extern S2LogFile* g_pLog;