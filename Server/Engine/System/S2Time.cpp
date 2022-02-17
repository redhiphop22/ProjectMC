#include "S2Engine.h"
#include "S2Time.h"
#include <mmsystem.h>

#define SERVER_BEGIN_TIME	10000

S2ThreadTimer* s_pTimer = nullptr;

S2ThreadTimer::S2ThreadTimer()
{
}

S2ThreadTimer::~S2ThreadTimer()
{
	Destroy();
}

bool S2ThreadTimer::Create()
{
	auto threadProc = [this]() -> void {
		this->OnUpdate();
	};
	return m_thread.ThreadCreateFunc(threadProc);
}

void S2ThreadTimer::Destroy()
{
	m_thread.DestroyThread();
}

void S2ThreadTimer::OnUpdate()
{
	THREAD_UPDATE_START;

	SYSTEMTIME stSystemTime;

	// 썸머타임 영향받는 로컬 년월일시분초 업데이트		
	//::GetLocalTime(&stSystemTime);
	//m_dt32Local = stSystemTime;

	// 썸머타임 관계없는 표준 년월일시분초 업데이트
	//SYSTEMTIME UTCTime;
	//TIME_ZONE_INFORMATION TZInfo;
	//::GetSystemTime(&UTCTime);
	//::GetTimeZoneInformation(&TZInfo);
	//TZInfo.DaylightBias = 0;			// 타임존 정보에서 썸머타임을 적용하지 않도록 보정값 변경
	//::SystemTimeToTzSpecificLocalTime(&TZInfo, &UTCTime, &stSystemTime);
	//m_dt32Stand = stSystemTime;

	//// 4. 슬립
	//Waiting(1);

	THREAD_UPDATE_END;
}

bool S2Time::Init()
{
	s_pTimer = new S2ThreadTimer;
	return s_pTimer->Create();
}

void S2Time::Destroy()
{
	SAFE_DELETE(s_pTimer);
}

date32_t S2Time::LocalTime()
{
	return s_pTimer->GetLocalTime();
}

date32_t S2Time::StandTime()
{
	return 0;//s_pTimer->GetStandTime();
}
