#pragma once

class IocpSessionMgr : public s2::S2IocpSessionMgr
{
public:
	IocpSessionMgr();
	virtual ~IocpSessionMgr();

	virtual bool			Create(int32_t sessionCount) override;

};