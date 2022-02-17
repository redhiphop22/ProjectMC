#pragma once

#include "ControllerBase.h"

class CClientApp;

class ControllerSingle : public ControllerBase
{
public:
	ControllerSingle();
	~ControllerSingle();

	inline static ControllerSingle& GetInstance()
	{
		static ControllerSingle instance;
		return instance;
	}

	void					SetApp(CClientApp* app);

public:
	virtual void			CONNECT_RESULT_S2C(User* user, const char* buffer, int32_t size) override;
	virtual void			SERVER_CONNECT_S2C(User* user, const char* buffer, int32_t size) override;
	virtual void			CHARACTER_INFO_S2C(User* user, const char* buffer, int32_t size) override;
	virtual void			CHARACTER_NAME_DUPLICATION_S2C(User* user, const char* buffer, int32_t size) override;
	virtual void			CHARACTER_CREATE_S2C(User* user, const char* buffer, int32_t size) override;

private:
	CClientApp*				m_app = nullptr;
};

#define CONTROLLER_S ControllerSingle::GetInstance()