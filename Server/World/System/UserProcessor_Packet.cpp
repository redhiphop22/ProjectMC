#include "World.h"
#include "UserProcessor.h"
#include "UserInfo.h"
#include "UserMgr.h"
#include "IocpSession.h"

bool UserProcessor::RegisterPacket_Packet()
{
	REGIST_PACKET(m_packetFunc.emplace(std::make_pair(protocol::MESSAGE_SERVER_CONNECT_C2S, &UserProcessor::SERVER_CONNECT_C2S)));
	REGIST_PACKET(m_packetFunc.emplace(std::make_pair(protocol::MESSAGE_ENTER_LOBBY_C2S, &UserProcessor::ENTER_LOBBY_C2S)));
	REGIST_PACKET(m_packetFunc.emplace(std::make_pair(protocol::MESSAGE_CHARACTER_INFO_C2S, &UserProcessor::CHARACTER_INFO_C2S)));
	REGIST_PACKET(m_packetFunc.emplace(std::make_pair(protocol::MESSAGE_CHARACTER_NAME_DUPLICATION_C2S, &UserProcessor::CHARACTER_NAME_DUPLICATION_C2S)));
	REGIST_PACKET(m_packetFunc.emplace(std::make_pair(protocol::MESSAGE_CHARACTER_CREATE_C2S, &UserProcessor::CHARACTER_CREATE_C2S)));
	REGIST_PACKET(m_packetFunc.emplace(std::make_pair(protocol::MESSAGE_ENTER_MAP_C2S, &UserProcessor::ENTER_MAP_C2S)));
	REGIST_PACKET(m_packetFunc.emplace(std::make_pair(protocol::MESSAGE_ENTITY_MOVE_VELOCITY_C2S, &UserProcessor::ENTITY_MOVE_VELOCITY_C2S)));
	REGIST_PACKET(m_packetFunc.emplace(std::make_pair(protocol::MESSAGE_ENTITY_MOVE_STOP_C2S, &UserProcessor::ENTITY_MOVE_STOP_C2S)));

	return true;
}

bool UserProcessor::SERVER_CONNECT_C2S(IocpSession* session, const char* buffer, int32_t size)
{
	PACKET_CONVERT_C2S(SERVER_CONNECT_C2S, SERVER_CONNECT_S2C, buffer, size);

	if(nullptr == msg->id())
	{
		SendError(common::RESULT_CODE_ERROR_PACKET_VERIFIER);
		return false;
	}

	uid = msg->uid();
	std::string id = msg->id()->data();
	
	if(false == m_userMgr->InsertWaittingUser(uid, id, session))
	{
		SendError(common::RESULT_CODE_ERROR_PACKET_MESSAGE);
		return false; 
	}

	return true;
}

bool UserProcessor::ENTER_LOBBY_C2S(IocpSession* session, const char* buffer, int32_t size)
{
	PACKET_CONVERT_C2S(ENTER_LOBBY_C2S, ENTER_LOBBY_S2C, buffer, size);

	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
	auto body = protocol::CreateENTER_LOBBY_S2C(fbb, common::RESULT_CODE_SUCCESS);
	fbb.Finish(body);

	session->SendPacket(protocol::MESSAGE_ENTER_LOBBY_S2C, fbb);

	return true;
}

bool UserProcessor::CHARACTER_INFO_C2S(IocpSession* session, const char* buffer, int32_t size)
{
	PACKET_CONVERT_C2S(CHARACTER_INFO_C2S, CHARACTER_INFO_S2C, buffer, size);

	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
	auto body = protocol_svr::CreateCHARACTER_INFO_DB_REQ(fbb, uid);
	fbb.Finish(body);

	if(false == SendPacket_DB(protocol_svr::MESSAGE_CHARACTER_INFO_DB_REQ, fbb))
	{
		SendError(common::RESULT_CODE_ERROR_PACKET_MESSAGE);
		return false; 
	}

	return true;
}

bool UserProcessor::CHARACTER_NAME_DUPLICATION_C2S(IocpSession* session, const char* buffer, int32_t size)
{
	PACKET_CONVERT_C2S(CHARACTER_NAME_DUPLICATION_C2S, CHARACTER_NAME_DUPLICATION_S2C, buffer, size);

	if(nullptr == msg->nick_name())
	{
		SendError(common::RESULT_CODE_ERROR_PACKET_VERIFIER);
		return false;
	}

	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
	auto body = protocol_svr::CreateCHARACTER_NAME_DUPLICATION_DB_REQ(fbb, 
		uid,
		fbb.CreateString(msg->nick_name()));
	fbb.Finish(body);

	if(false == SendPacket_DB(protocol_svr::MESSAGE_CHARACTER_NAME_DUPLICATION_DB_REQ, fbb))
	{
		SendError(common::RESULT_CODE_ERROR_PACKET_MESSAGE);
		return false; 
	}

	return true;
}

bool UserProcessor::CHARACTER_CREATE_C2S(IocpSession* session, const char* buffer, int32_t size)
{
	PACKET_CONVERT_C2S(CHARACTER_CREATE_C2S, CHARACTER_CREATE_S2C, buffer, size);
	
	if(nullptr == msg->nick_name() ||
		nullptr == msg->face() ||
		nullptr == msg->equipment())
	{
		SendError(common::RESULT_CODE_ERROR_PACKET_VERIFIER);
		return false; 
	}

	CharacterFace face(msg->face());
	CharacterEquipmentList equipmentList(msg->equipment());

	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
	auto body = protocol_svr::CreateCHARACTER_CREATE_DB_REQ(fbb, 
		uid,
		fbb.CreateString(msg->nick_name()),
		msg->type(),
		msg->belong(),
		face.Packing(fbb),
		fbb.CreateVector(equipmentList.Packing(fbb)));
	fbb.Finish(body);

	if(false == SendPacket_DB(protocol_svr::MESSAGE_CHARACTER_CREATE_DB_REQ, fbb))
	{
		SendError(common::RESULT_CODE_ERROR_PACKET_MESSAGE);
		return false; 
	}

	return true;
}

bool UserProcessor::ENTER_MAP_C2S(IocpSession* session, const char* buffer, int32_t size)
{
	PACKET_CONVERT_C2S(ENTER_MAP_C2S, ENTER_MAP_S2C, buffer, size);

	auto user = m_userMgr->GetUserInfo(uid);
	if(nullptr == user)
		return false;

	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
	auto body = protocol_svr::CreateENTER_MAP_REQ(fbb, 
		uid,
		msg->map_id(),
		user->MakePacket_Entity(fbb)
	);
	fbb.Finish(body);

	if(false == SendPacket_Zone(protocol_svr::MESSAGE_ENTER_MAP_REQ, fbb))
	{
		SendError(common::RESULT_CODE_ERROR_PACKET_MESSAGE);
		return false; 
	}

	return true;
}

bool UserProcessor::ENTITY_MOVE_VELOCITY_C2S(IocpSession* session, const char* buffer, int32_t size)
{
	PACKET_CONVERT_C2S(ENTITY_MOVE_VELOCITY_C2S, ENTITY_MOVE_VELOCITY_S2C, buffer, size);

	if(nullptr == msg->position() ||
		nullptr == msg->forward())
	{
		SendError(common::RESULT_CODE_ERROR_PACKET_VERIFIER);
		return false; 
	}

	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
	auto body = protocol_svr::CreateENTITY_MOVE_VELOCITY_REQ(fbb, 
		uid,
		msg->excute_time(),
		msg->position(),
		msg->forward(),
		msg->speed()
	);
	fbb.Finish(body);

	if(false == SendPacket_Zone(protocol_svr::MESSAGE_ENTITY_MOVE_VELOCITY_REQ, fbb))
	{
		SendError(common::RESULT_CODE_ERROR_PACKET_MESSAGE);
		return false; 
	}

	return true;
}

bool UserProcessor::ENTITY_MOVE_STOP_C2S(IocpSession* session, const char* buffer, int32_t size)
{
	PACKET_CONVERT_C2S(ENTITY_MOVE_STOP_C2S, ENTITY_MOVE_STOP_S2C, buffer, size);

	if(nullptr == msg->position())
	{
		SendError(common::RESULT_CODE_ERROR_PACKET_VERIFIER);
		return false; 
	}

	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
	auto body = protocol_svr::CreateENTITY_MOVE_STOP_REQ(fbb, 
		uid,
		msg->excute_time(),
		msg->position()
	);
	fbb.Finish(body);

	if(false == SendPacket_Zone(protocol_svr::MESSAGE_ENTITY_MOVE_STOP_REQ, fbb))
	{
		SendError(common::RESULT_CODE_ERROR_PACKET_MESSAGE);
		return false; 
	}

	return true;
}