#include "World.h"
#include "UserInfo.h"
#include "IocpSession.h"

UserInfo::UserInfo(IocpSession* session, uid_t uid, common::ACCOUNT_AUTHORITY authority)
	: m_session(session)
{	
	m_session->CompletionLogin(uid);
}

UserInfo::~UserInfo()
{
}

void UserInfo::Destroy()
{
	printf("[Succ] User Logout (ID:%s)\n", m_nickName.c_str());
	m_session->RegisterAccept();
}

bool UserInfo::SendPacket(protocol::MESSAGE messageID, flatbuffers::FlatBufferBuilder& fbb)
{
	return m_session->SendPacket(messageID, fbb);
}

bool UserInfo::CHARACTER_INFO_DB_ACK(const protocol_svr::CHARACTER_INFO_DB_ACK* msg)
{
	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);

	auto result = msg->result();
	if(common::RESULT_CODE_SUCESS == result)
	{
		m_nickName = msg->nick_name()->c_str();
		m_characterType = msg->type();
		m_belong = msg->belong();
		m_faceInfo.UnPacking(*msg->face());
		m_equipmentList.UnPacking(msg->equipment());
	}

	auto body = protocol::CreateCHARACTER_INFO_S2C(fbb,
		result,
		fbb.CreateString(m_nickName),
		m_characterType,
		m_belong,
		m_faceInfo.Packing(fbb),
		fbb.CreateVector(m_equipmentList.Packing(fbb))
	);
	fbb.Finish(body);

	SendPacket(protocol::MESSAGE_CHARACTER_INFO_S2C, fbb);

	printf("[Succ] User Login (ID:%s)\n", m_nickName.c_str());

	return true;
}

bool UserInfo::CHARACTER_CREATE_DB_ACK(const protocol_svr::CHARACTER_CREATE_DB_ACK* msg)
{
	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);

	auto result = msg->result();
	if(common::RESULT_CODE_SUCESS == result)
	{
		m_nickName = msg->nick_name()->c_str();
		m_characterType = msg->type();
		m_belong = msg->belong();
		m_faceInfo.UnPacking(*msg->face());
		m_equipmentList.UnPacking(msg->equipment());
	}

	auto body = protocol::CreateCHARACTER_CREATE_S2C(fbb,
		result,
		fbb.CreateString(m_nickName),
		m_characterType,
		m_belong,
		m_faceInfo.Packing(fbb),
		fbb.CreateVector(m_equipmentList.Packing(fbb))
	);
	fbb.Finish(body);

	SendPacket(protocol::MESSAGE_CHARACTER_CREATE_S2C, fbb);

	return true;
}