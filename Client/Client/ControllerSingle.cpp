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

	if(common::RESULT_CODE_SUCCESS == msg->result())
	{
		m_app->SwitchView(VIEW_PAGE::SERVERMOVE);
	}
}

void ControllerSingle::SERVER_CONNECT_S2C(User* user, const char* buffer, int32_t size)
{
	PACKET_CONVERT(SERVER_CONNECT_S2C, buffer, size);

	if(common::RESULT_CODE_SUCCESS == msg->result())
	{
		((ClientServerMove*)m_app->GetActivePage())->SERVER_CONNECT_S2C(msg);
		m_app->SwitchView(VIEW_PAGE::LOBBY);
	}
}

void ControllerSingle::ENTER_LOBBY_S2C(User* user, const char* buffer, int32_t size)
{
	PACKET_CONVERT(ENTER_LOBBY_S2C, buffer, size);

	((ClientLobby*)m_app->GetActivePage())->ENTER_LOBBY_S2C(msg);
}

void ControllerSingle::CHARACTER_INFO_S2C(User* user, const char* buffer, int32_t size)
{
	PACKET_CONVERT(CHARACTER_INFO_S2C, buffer, size);

	if(common::RESULT_CODE_SUCCESS == msg->result())
	{
		((ClientLobby*)m_app->GetActivePage())->CHARACTER_INFO_S2C(msg);
	}
	else if(common::RESULT_CODE_SUCCESS_CHARACTER_NONE == msg->result())
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

	if(common::RESULT_CODE_SUCCESS == msg->result())
	{
		m_app->SwitchView(VIEW_PAGE::LOBBY);
	}
}

void ControllerSingle::ENTER_MAP_S2C(User* user, const char* buffer, int32_t size)
{
	PACKET_CONVERT(ENTER_MAP_S2C, buffer, size);

	if(common::RESULT_CODE_SUCCESS == msg->result())
	{
		m_app->SwitchView(VIEW_PAGE::MAP);
	}
}

void ControllerSingle::ENTITY_SPAWN_S2C(User* user, const char* buffer, int32_t size)
{
	PACKET_CONVERT(ENTITY_SPAWN_S2C, buffer, size);

	((ClientMap*)m_app->GetActivePage())->ENTITY_SPAWN_S2C(msg);
}

void ControllerSingle::ENTITY_DESTROY_S2C(User* user, const char* buffer, int32_t size)
{
	PACKET_CONVERT(ENTITY_DESTROY_S2C, buffer, size);

	((ClientMap*)m_app->GetActivePage())->ENTITY_DESTROY_S2C(msg);
}

void ControllerSingle::ENTITY_MOVE_VELOCITY_S2C(User* user, const char* buffer, int32_t size)
{
	PACKET_CONVERT(ENTITY_MOVE_VELOCITY_S2C, buffer, size);

	((ClientMap*)m_app->GetActivePage())->ENTITY_MOVE_VELOCITY_S2C(msg);
}

void ControllerSingle::ENTITY_MOVE_STOP_S2C(User* user, const char* buffer, int32_t size)
{
	PACKET_CONVERT(ENTITY_MOVE_STOP_S2C, buffer, size);

	((ClientMap*)m_app->GetActivePage())->ENTITY_MOVE_STOP_S2C(msg);
}