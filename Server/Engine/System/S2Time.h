#pragma once

class S2ThreadTimer : public s2::S2Thread
{
	date32_t m_dt32Local;			// 썸머타임 적용되는 로컬시(변환후)
	date32_t m_dt32Stand;			// 썸머타임 무시하는 표준시(변환후)

public:
	S2ThreadTimer();
	~S2ThreadTimer();

	virtual bool Create();
	virtual void Destroy();

	date32_t GetLocalTime() { return m_dt32Local; }
	date32_t GetStandTime() { return m_dt32Stand; }

protected:
	virtual bool OnThreadUpdate() override;
};

extern S2ThreadTimer* s_pTimer;

namespace S2Time
{
	bool				Init();
	void				Destroy();
	inline date32_t	LocalTime();
	inline date32_t	StandTime();
};
