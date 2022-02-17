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
	RegisterPacket_Packet();
	RegisterPacket_Zone();

	REGIST_PACKET(m_dbFunc.emplace(std::make_pair(protocol_svr::MESSAGE_SERVER_CONNECT_DB_ACK, &UserProcessor::SERVER_CONNECT_DB_ACK)));
	REGIST_PACKET(m_dbFunc.emplace(std::make_pair(protocol_svr::MESSAGE_CHARACTER_INFO_DB_ACK, &UserProcessor::CHARACTER_INFO_DB_ACK)));
	REGIST_PACKET(m_dbFunc.emplace(std::make_pair(protocol_svr::MESSAGE_CHARACTER_NAME_DUPLICATION_DB_ACK, &UserProcessor::CHARACTER_NAME_DUPLICATION_DB_ACK)));
	REGIST_PACKET(m_dbFunc.emplace(std::make_pair(protocol_svr::MESSAGE_CHARACTER_CREATE_DB_ACK, &UserProcessor::CHARACTER_CREATE_DB_ACK)));

	return true;
}

bool UserProcessor::Create()
{
	RegisterPacket();

	if(false == MESSAGE_PROCESSOR().SetReceiver(MessageProcessor::MESSAGE_TYPE::USER, this))
	{
		return false;
	}

	m_userMgr = new UserMgr;
	if(nullptr == m_userMgr ||
		false == m_userMgr->Create())
	{
		return false;
	}	

	auto threadProc = [this]() -> void {
		this->OnUpdate();
	};
	return m_thread.ThreadCreateFunc(threadProc);
}

void UserProcessor::Destroy()
{
	m_thread.DestroyThread();

	if(m_userMgr)
	{
		m_userMgr->Destroy();
		delete m_userMgr;
		m_userMgr = nullptr;
	}
}

void UserProcessor::OnUpdate()
{
	THREAD_UPDATE_START;

	MESSAGE_PROCESSOR().OnUpdate(MessageProcessor::MESSAGE_TYPE::USER);
	
	m_userMgr->OnUpdate();

	THREAD_UPDATE_END;
}

bool UserProcessor::OnMessageUpdate(int32_t groupIdx, void* data)
{
	PacketParsing(groupIdx, (char*)data);

	return true;
}

void UserProcessor::PacketParsing(int32_t groupIdx, char* buffer)
{
	switch(static_cast<MessageProcessor::MESSAGE_GROUP_USER>(groupIdx))
	{
	case MessageProcessor::MESSAGE_GROUP_USER::IOCP:
		{
			PacketParsing_IOCP(buffer);
		}
		break;
	case MessageProcessor::MESSAGE_GROUP_USER::DATABASE:
		{
			PacketParsing_Database(buffer);
		}
		break;
	case MessageProcessor::MESSAGE_GROUP_USER::ZONE:
		{
			PacketParsing_Zone(buffer);
		}
		break;
	}
}

void UserProcessor::PacketParsing_IOCP(char* buffer)
{
	MessageProcessor::MessageUser* user = (MessageProcessor::MessageUser*)buffer;
	
	char* packetBuffer = user->m_data;
	s2::packet_size_t packetSize = *(s2::packet_size_t*)(&packetBuffer[0]);
	s2::packet_protocol_t packetProtocol = *(s2::packet_protocol_t*)(&packetBuffer[2]);

	const char* body = &packetBuffer[6];
	auto iter = m_packetFunc.find(static_cast<protocol::MESSAGE>(packetProtocol));
	if(iter == m_packetFunc.end())
	{
		flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
		auto body = protocol::CreateRESULT_S2C(fbb, packetProtocol, common::RESULT_CODE_ERROR_PACKET_PROTOCOL);
		fbb.Finish(body);
		user->m_session->SendPacket(protocol::MESSAGE_RESULT_S2C, fbb);
		return;
	}
	if(false == (this->*(iter->second))(user->m_session, body, packetSize - PACKET_HEAD_SIZE))
	{
		return;
	}
}

void UserProcessor::PacketParsing_Database(char* buffer)
{
	MessageProcessor::MessageBase* user = (MessageProcessor::MessageBase*)buffer;
	
	char* packetBuffer = user->m_data;
	s2::packet_size_t packetSize = *(s2::packet_size_t*)(&packetBuffer[0]); \
	s2::packet_protocol_t packetProtocol = *(s2::packet_protocol_t*)(&packetBuffer[2]); \

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

void UserProcessor::PacketParsing_Zone(char* buffer)
{
	MessageProcessor::MessageBase* user = (MessageProcessor::MessageBase*)buffer;
	
	char* packetBuffer = user->m_data;
	s2::packet_size_t packetSize = *(s2::packet_size_t*)(&packetBuffer[0]); \
	s2::packet_protocol_t packetProtocol = *(s2::packet_protocol_t*)(&packetBuffer[2]); \

	const char* body = &packetBuffer[4];
	auto iter = m_zoneFunc.find(static_cast<protocol_svr::MESSAGE>(packetProtocol));
	if (iter == m_zoneFunc.end())
	{
		return;
	}
	if (false == (this->*(iter->second))(body, packetSize - 4))
	{
		return;
	}
}

bool UserProcessor::SendPacket_DB(protocol_svr::MESSAGE message, flatbuffers::FlatBufferBuilder& fbb)
{
	return MESSAGE_PROCESSOR().SnedPacket_DB(0, 0, message, fbb);
}

bool UserProcessor::SendPacket_Zone(protocol_svr::MESSAGE message, flatbuffers::FlatBufferBuilder& fbb)
{
	return MESSAGE_PROCESSOR().SnedPacket_Zone(0, 0, message, fbb);
}