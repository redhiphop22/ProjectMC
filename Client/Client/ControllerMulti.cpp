#include "pch.h"
#include "ControllerMulti.h"
#include "User.h"

ControllerMulti::ControllerMulti()
{
}

ControllerMulti::~ControllerMulti()
{
}

void ControllerMulti::CONNECT_RESULT_S2C(User* user, const char* buffer, int32_t size)
{
	PACKET_CONVERT(CONNECT_RESULT_S2C, buffer, size);

	if(common::RESULT_CODE_SUCCESS == msg->result())
	{
		flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
		auto body = protocol::CreateSERVER_CONNECT_C2S(fbb,
			user->m_uid,
			fbb.CreateString(user->m_id)
		);
		fbb.Finish(body);
	
		SERVER_CONNECT_C2S(user, fbb);
	}
}

void ControllerMulti::SERVER_CONNECT_S2C(User* user, const char* buffer, int32_t size)
{
	PACKET_CONVERT(SERVER_CONNECT_S2C, buffer, size);

	if(common::RESULT_CODE_SUCCESS == msg->result())
	{
		flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
		auto body = protocol::CreateENTER_LOBBY_C2S(fbb);
		fbb.Finish(body);
	
		ENTER_LOBBY_C2S(user, fbb);
	}
}

void ControllerMulti::ENTER_LOBBY_S2C(User* user, const char* buffer, int32_t size)
{
	PACKET_CONVERT(ENTER_LOBBY_S2C, buffer, size);

	if(common::RESULT_CODE_SUCCESS == msg->result())
	{
		flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
		auto body = protocol::CreateCHARACTER_INFO_C2S(fbb);
		fbb.Finish(body);
	
		CHARACTER_INFO_C2S(user, fbb);
	}
}

void ControllerMulti::CHARACTER_INFO_S2C(User* user, const char* buffer, int32_t size)
{
	PACKET_CONVERT(CHARACTER_INFO_S2C, buffer, size);

	if(common::RESULT_CODE_SUCCESS == msg->result())
	{
		user->DisConnect();
	}
	else if(common::RESULT_CODE_SUCCESS_CHARACTER_NONE == msg->result())
	{
		user->m_nick = user->m_id;

		flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
		user->MakeCHARACTER_CREATE_C2S(fbb);
		CHARACTER_CREATE_C2S(user, fbb);
	}
}

void ControllerMulti::CHARACTER_CREATE_S2C(User* user, const char* buffer, int32_t size)
{
	PACKET_CONVERT(CHARACTER_CREATE_S2C, buffer, size);

	if(common::RESULT_CODE_SUCCESS == msg->result())
	{
		flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
		auto body = protocol::CreateCHARACTER_INFO_C2S(fbb);
		fbb.Finish(body);
	
		CHARACTER_INFO_C2S(user, fbb);
	}
}

void ControllerMulti::ENTER_MAP_S2C(User* user, const char* buffer, int32_t size)
{
	PACKET_CONVERT(ENTER_MAP_S2C, buffer, size);

	if(common::RESULT_CODE_SUCCESS == msg->result())
	{
		//flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
		//auto body = protocol::CreateCHARACTER_INFO_C2S(fbb);
		//fbb.Finish(body);
	
		//CHARACTER_INFO_C2S(user, fbb);
	}
}

void ControllerMulti::ENTITY_SPAWN_S2C(User* user, const char* buffer, int32_t size)
{
	PACKET_CONVERT(ENTITY_SPAWN_S2C, buffer, size);
}

void ControllerMulti::ENTITY_DESTROY_S2C(User* user, const char* buffer, int32_t size)
{
	PACKET_CONVERT(ENTITY_DESTROY_S2C, buffer, size);
}

void ControllerMulti::ENTITY_MOVE_VELOCITY_S2C(User* user, const char* buffer, int32_t size)
{
	PACKET_CONVERT(ENTITY_MOVE_VELOCITY_S2C, buffer, size);
}

void ControllerMulti::ENTITY_MOVE_STOP_S2C(User* user, const char* buffer, int32_t size)
{
	PACKET_CONVERT(ENTITY_MOVE_STOP_S2C, buffer, size);
}