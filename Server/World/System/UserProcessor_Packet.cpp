#include "World.h"
#include "UserProcessor.h"
#include "UserInfo.h"
#include "UserMgr.h"
#include "IocpSession.h"

bool UserProcessor::SERVER_CONNECT_C2S(IocpSession* session, const char* buffer, int32_t size)
{
	PACKET_CONVERT_C2S(SERVER_CONNECT_C2S, buffer, size);

	uid_t uid = msg->uid();
	std::string id = msg->id()->data();
	
	return m_userMgr->InsertWaittingUser(uid, id, session);
}

bool UserProcessor::CHARACTER_INFO_C2S(IocpSession* session, const char* buffer, int32_t size)
{
	PACKET_CONVERT_C2S(CHARACTER_INFO_C2S, buffer, size);
	
	uid_t uid = session->GetUID();

	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
	auto body = protocol_svr::CreateCHARACTER_INFO_DB_REQ(fbb, uid);
	fbb.Finish(body);

	SendPacket_DB(protocol_svr::MESSAGE_CHARACTER_INFO_DB_REQ, fbb);

	return true;
}

bool UserProcessor::CHARACTER_NAME_DUPLICATION_C2S(IocpSession* session, const char* buffer, int32_t size)
{
	PACKET_CONVERT_C2S(CHARACTER_NAME_DUPLICATION_C2S, buffer, size);
	
	uid_t uid = session->GetUID();

	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
	auto body = protocol_svr::CreateCHARACTER_NAME_DUPLICATION_DB_REQ(fbb, 
		uid,
		fbb.CreateString(msg->nick_name()));
	fbb.Finish(body);

	SendPacket_DB(protocol_svr::MESSAGE_CHARACTER_NAME_DUPLICATION_DB_REQ, fbb);

	return true;
}

bool UserProcessor::CHARACTER_CREATE_C2S(IocpSession* session, const char* buffer, int32_t size)
{
	PACKET_CONVERT_C2S(CHARACTER_CREATE_C2S, buffer, size);
	
	uid_t uid = session->GetUID();
	UserInfo::CharacterFace face;
	UserInfo::CharacterEquipmentList equipmentList;
	face.UnPacking(*msg->face());
	equipmentList.UnPacking(msg->equipment());

	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
	auto body = protocol_svr::CreateCHARACTER_CREATE_DB_REQ(fbb, 
		uid,
		fbb.CreateString(msg->nick_name()),
		msg->type(),
		msg->belong(),
		face.Packing(fbb),
		fbb.CreateVector(equipmentList.Packing(fbb)));
	fbb.Finish(body);

	SendPacket_DB(protocol_svr::MESSAGE_CHARACTER_CREATE_DB_REQ, fbb);

	return true;
}

bool UserProcessor::MAP_ENTER_C2S(IocpSession* session, const char* buffer, int32_t size)
{
	return true;
}