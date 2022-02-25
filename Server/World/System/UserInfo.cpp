#include "World.h"
#include "UserInfo.h"
#include "IocpSession.h"

UserInfo::UserInfo(IocpSession* session, uid_t uid, common::ACCOUNT_AUTHORITY authority)
	: m_session(session)
	, m_uid(uid)
{	
	m_session->CompletionLogin(uid);
}

UserInfo::~UserInfo()
{
}

void UserInfo::Destroy()
{
	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
	auto body = protocol_svr::CreateLOGOUT_REQ(fbb,
		m_uid,
		m_mapId
	);
	fbb.Finish(body);
	MESSAGE_PROCESSOR().SnedPacket_Zone(0, 0, protocol_svr::MESSAGE_LOGOUT_REQ, fbb);

	printf("[Succ] User Logout (ID:%s)\n", m_nickName.c_str());
	m_session->RegisterAccept();
}

bool UserInfo::SendPacket(protocol::MESSAGE messageID, flatbuffers::FlatBufferBuilder& fbb)
{
	return m_session->SendPacket(messageID, fbb);
}

flatbuffers::Offset<common::ENTITY_INFO> UserInfo::MakePacket_Entity(flatbuffers::FlatBufferBuilder& fbb)
{
	return common::CreateENTITY_INFO(fbb,
		m_uid,
		fbb.CreateString(m_nickName.c_str()),
		m_faceInfo.Packing(fbb),
		fbb.CreateVector(m_equipmentList.Packing(fbb))
	);
}

bool UserInfo::CHARACTER_INFO_DB_ACK(const protocol_svr::CHARACTER_INFO_DB_ACK* msg)
{
	auto result = msg->result();
	if(common::RESULT_CODE_SUCCESS == result)
	{
		m_nickName = msg->nick_name()->c_str();
		m_characterType = msg->type();
		m_belong = msg->belong();
		m_faceInfo.UnPacking(msg->face());
		m_equipmentList.UnPacking(msg->equipment());
	}

	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
	auto body = protocol::CreateCHARACTER_INFO_S2C(fbb,
		result,
		fbb.CreateString(m_nickName),
		m_characterType,
		m_belong,
		m_faceInfo.Packing(fbb),
		fbb.CreateVector(m_equipmentList.Packing(fbb)),
		msg->last_map_id(),
		msg->last_map_position()
	);
	fbb.Finish(body);

	SendPacket(protocol::MESSAGE_CHARACTER_INFO_S2C, fbb);

	printf("[Succ] User Login (ID:%s)\n", m_nickName.c_str());

	return true;
}

bool UserInfo::CHARACTER_CREATE_DB_ACK(const protocol_svr::CHARACTER_CREATE_DB_ACK* msg)
{
	auto result = msg->result();
	if(common::RESULT_CODE_SUCCESS == result)
	{
		m_nickName = msg->nick_name()->c_str();
		m_characterType = msg->type();
		m_belong = msg->belong();
		m_faceInfo.UnPacking(msg->face());
		m_equipmentList.UnPacking(msg->equipment());
	}

	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
	auto body = protocol::CreateCHARACTER_CREATE_S2C(fbb,
		result,
		fbb.CreateString(m_nickName),
		m_characterType,
		m_belong,
		m_faceInfo.Packing(fbb),
		fbb.CreateVector(m_equipmentList.Packing(fbb)),
		msg->last_map_id(),
		msg->last_map_position()
	);
	fbb.Finish(body);

	SendPacket(protocol::MESSAGE_CHARACTER_CREATE_S2C, fbb);

	return true;
}

bool UserInfo::ENTER_MAP_ACK(const protocol_svr::ENTER_MAP_ACK* msg)
{
	if(common::RESULT_CODE::RESULT_CODE_SUCCESS == msg->result())
	{
		m_mapId = msg->map_id();
	}

	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
	auto body = protocol::CreateENTER_MAP_S2C(fbb,
		msg->result(),
		msg->map_id(),
		s2::S2Time::Now()
	);
	fbb.Finish(body);

	SendPacket(protocol::MESSAGE_ENTER_MAP_S2C, fbb);

	return true;
}