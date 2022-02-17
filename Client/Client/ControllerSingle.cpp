#include "pch.h"
#include "ControllerSingle.h"
#include "Client.h"
#include "ClientInclude.h"

ControllerSingle::ControllerSingle()
{
}

ControllerSingle::~ControllerSingle()
{
}

void ControllerSingle::SetApp(CClientApp* app)
{
	m_app = app;
}

void ControllerSingle::CONNECT_RESULT_S2C(User* user, const char* buffer, int32_t size)
{
	PACKET_CONVERT(CONNECT_RESULT_S2C, buffer, size);

	if(common::RESULT_CODE_SUCESS == msg->result())
	{
		m_app->SwitchView(VIEW_PAGE::SERVERMOVE);
	}
}

void ControllerSingle::SERVER_CONNECT_S2C(User* user, const char* buffer, int32_t size)
{
	PACKET_CONVERT(SERVER_CONNECT_S2C, buffer, size);

	if(common::RESULT_CODE_SUCESS == msg->result())
	{
		m_app->SwitchView(VIEW_PAGE::LOBBY);
	}
}

void ControllerSingle::CHARACTER_INFO_S2C(User* user, const char* buffer, int32_t size)
{
	PACKET_CONVERT(CHARACTER_INFO_S2C, buffer, size);

	if(common::RESULT_CODE_SUCESS == msg->result())
	{
		((ClientLobby*)m_app->GetActivePage())->SetCharacterInfo(msg);
		//m_app->SwitchView(VIEW_PAGE::LOBBY);
	}
	else if(common::RESULT_CODE_SUCESS_CHARACTER_NONE == msg->result())
	{
		// 캐릭터 생성
		m_app->SwitchView(VIEW_PAGE::CREATE_CHARACTER);
	}
}

void ControllerSingle::CHARACTER_NAME_DUPLICATION_S2C(User* user, const char* buffer, int32_t size)
{
	PACKET_CONVERT(CHARACTER_NAME_DUPLICATION_S2C, buffer, size);

	((ClientCreateCharacter*)m_app->GetActivePage())->NameDuplicationResult(msg);
}

void ControllerSingle::CHARACTER_CREATE_S2C(User* user, const char* buffer, int32_t size)
{
	PACKET_CONVERT(CHARACTER_CREATE_S2C, buffer, size);

	if(common::RESULT_CODE_SUCESS == msg->result())
	{
		m_app->SwitchView(VIEW_PAGE::LOBBY);
	}
}