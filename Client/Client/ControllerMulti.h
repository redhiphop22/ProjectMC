#pragma once

#include "ControllerBase.h"

class ControllerMulti : public ControllerBase
{
public:
	ControllerMulti();
	~ControllerMulti();

	inline static ControllerMulti& GetInstance()
	{
		static ControllerMulti instance;
		return instance;
	}

public:
	virtual void			CONNECT_RESULT_S2C(User* user, const char* buffer, int32_t size) override;
	virtual void			SERVER_CONNECT_S2C(User* user, const char* buffer, int32_t size) override;
	virtual void			CHARACTER_INFO_S2C(User* user, const char* buffer, int32_t size) override;
	virtual void			CHARACTER_CREATE_S2C(User* user, const char* buffer, int32_t size) override;
};

#define CONTROLLER_M ControllerMulti::GetInstance()