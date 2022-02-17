#include "World.h"
#include "UserProcessor.h"
#include "UserMgr.h"
#include "UserInfo.h"

bool UserProcessor::SERVER_CONNECT_DB_ACK(const char* buffer, int32_t size)
{
	PACKET_CONVERT_S2S(SERVER_CONNECT_DB_ACK, buffer, size);

	uid_t UID = msg->uid();

	if(common::RESULT_CODE::RESULT_CODE_SUCESS != msg->result())
	{
		m_userMgr->DeleteWaittingUser(UID);

		// 실패 처리
		return true;
	}

	return m_userMgr->InsertUser(msg);
}

bool UserProcessor::CHARACTER_INFO_DB_ACK(const char* buffer, int32_t size)
{
	PACKET_CONVERT_S2S(CHARACTER_INFO_DB_ACK, buffer, size);

	uid_t UID = msg->uid();
	auto session = m_userMgr->GetUserInfo(UID);
	if(nullptr == session)
		return false;

	session->CHARACTER_INFO_DB_ACK(msg);

	return true;	
}

bool UserProcessor::CHARACTER_NAME_DUPLICATION_DB_ACK(const char* buffer, int32_t size)
{
	PACKET_CONVERT_S2S(CHARACTER_NAME_DUPLICATION_DB_ACK, buffer, size);

	uid_t UID = msg->uid();
	auto session = m_userMgr->GetUserInfo(UID);
	if(nullptr == session)
		return false;

	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
	auto body = protocol::CreateCHARACTER_NAME_DUPLICATION_S2C(fbb,
		msg->result(),
		fbb.CreateString(msg->nick_name())
	);
	fbb.Finish(body);

	session->SendPacket(protocol::MESSAGE_CHARACTER_NAME_DUPLICATION_S2C, fbb);

	return true;	
}

bool UserProcessor::CHARACTER_CREATE_DB_ACK(const char* buffer, int32_t size)
{
	PACKET_CONVERT_S2S(CHARACTER_CREATE_DB_ACK, buffer, size);

	uid_t UID = msg->uid();
	auto session = m_userMgr->GetUserInfo(UID);
	if(nullptr == session)
		return false;

	session->CHARACTER_CREATE_DB_ACK(msg);

	return true;	
}