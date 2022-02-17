#include "World.h"
#include "UserProcessor.h"

#include "IocpSession.h"
#include "UserMgr.h"

UserProcessor::UserProcessor()
{
}

UserProcessor::~UserProcessor()
{
}

bool UserProcessor::RegisterPacket()
{
	REGIST_PACKET(m_packetFunc.emplace(std::make_pair(protocol::MESSAGE_SERVER_CONNECT_C2S, &UserProcessor::SERVER_CONNECT_C2S)));
	REGIST_PACKET(m_packetFunc.emplace(std::make_pair(protocol::MESSAGE_CHARACTER_INFO_C2S, &UserProcessor::CHARACTER_INFO_C2S)));
	REGIST_PACKET(m_packetFunc.emplace(std::make_pair(protocol::MESSAGE_CHARACTER_NAME_DUPLICATION_C2S, &UserProcessor::CHARACTER_NAME_DUPLICATION_C2S)));
	REGIST_PACKET(m_packetFunc.emplace(std::make_pair(protocol::MESSAGE_CHARACTER_CREATE_C2S, &UserProcessor::CHARACTER_CREATE_C2S)));

	REGIST_PACKET(m_dbFunc.emplace(std::make_pair(protocol_svr::MESSAGE_SERVER_CONNECT_DB_ACK, &UserProcessor::SERVER_CONNECT_DB_ACK)));
	REGIST_PACKET(m_dbFunc.emplace(std::make_pair(protocol_svr::MESSAGE_CHARACTER_INFO_DB_ACK, &UserProcessor::CHARACTER_INFO_DB_ACK)));
	REGIST_PACKET(m_dbFunc.emplace(std::make_pair(protocol_svr::MESSAGE_CHARACTER_NAME_DUPLICATION_DB_ACK, &UserProcessor::CHARACTER_NAME_DUPLICATION_DB_ACK)));
	REGIST_PACKET(m_dbFunc.emplace(std::make_pair(protocol_svr::MESSAGE_CHARACTER_CREATE_DB_ACK, &UserProcessor::CHARACTER_CREATE_DB_ACK)));

	return true;
}

bool UserProcessor::Create(int32_t workerCount, int32_t dbCount, int32_t bufferCount)
{
	if(false == MESSAGE_PROCESSOR().SetReceiver(MessageProcessor::MESSAGE_TYPE::USER, this))
	{
		return false;
	}

	if(false == MESSAGE_PROCESSOR().AddSender(MessageProcessor::MESSAGE_TYPE::DATABASE, 0, 0, 1000))
	{
		return false;
	}

	m_userMgr = new UserMgr;
	if(nullptr == m_userMgr ||
		false == m_userMgr->Create())
	{
		return false;
	}
	
	RegisterPacket();

	return ThreadCreate();
}

void UserProcessor::Destroy()
{
	s2::S2Thread::Destroy();

	m_userMgr->Destroy();
}

bool UserProcessor::OnThreadUpdate()
{
	MESSAGE_PROCESSOR().OnUpdate(MessageProcessor::MESSAGE_TYPE::USER);
	
	m_userMgr->OnUpdate();

	return true;
}

bool UserProcessor::OnMessageUpdate(int32_t groupIdx, void* data)
{
	PacketParsing(groupIdx, (char*)data);

	return true;
}

void UserProcessor::PacketParsing(int32_t groupIdx, char* buffer)
{
	MessageProcessor::MessageInfo_User* user = (MessageProcessor::MessageInfo_User*)buffer;
	
	char* packetBuffer = user->m_data;
	s2::packet_size_t packetSize = *(s2::packet_size_t*)(&packetBuffer[0]); \
	s2::packet_protocol_t packetProtocol = *(s2::packet_protocol_t*)(&packetBuffer[2]); \

	switch(static_cast<MessageProcessor::MESSAGE_GROUP_USER>(groupIdx))
	{
	case MessageProcessor::MESSAGE_GROUP_USER::NETWORK:
		{
			const char* body = &packetBuffer[6];
			auto iter = m_packetFunc.find(static_cast<protocol::MESSAGE>(packetProtocol));
			if (iter == m_packetFunc.end())
			{
				return;
			}
			if (false == (this->*(iter->second))(user->m_session, body, packetSize - PACKET_HEAD_SIZE))
			{
				return;
			}
		}
		break;
	case MessageProcessor::MESSAGE_GROUP_USER::DATABASE:
		{
			const char* body = &packetBuffer[4];
			auto iter = m_dbFunc.find(static_cast<protocol_svr::MESSAGE>(packetProtocol));
			if (iter == m_dbFunc.end())
			{
				return;
			}
			if (false == (this->*(iter->second))(body, packetSize - 4))
			{
				return;
			}
		}
		break;
	}
}

bool UserProcessor::SendPacket_DB(protocol_svr::MESSAGE message, flatbuffers::FlatBufferBuilder& fbb)
{
	return MESSAGE_PROCESSOR().SnedPacket_DB(0, 0, message, fbb);
}