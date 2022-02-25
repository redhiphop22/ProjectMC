#include "S2Engine.h"

namespace s2 {
bool S2Thread::SubCreateThread(bool useLog)
{
	if(useLog &&
		false == S2LOG_INSTANCE->PushThread(GetThreadId()))
	{
		S2LOG("Log Mgr Error");
		return false;
	}

	if(false == S2THREAD_INSTANCE().PushThread(GetThreadId(), this))
	{
		// 에러 로그만
		S2LOG("Thread Mgr Error");
		return false;
	}
	return true;
}
}