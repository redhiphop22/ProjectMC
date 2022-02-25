#include "Zone.h"
#include "UserProcessor.h"

#include "IocpSession.h"
#include "ZoneMgr.h"

UserProcessor::UserProcessor()
{
}

UserProcessor::~UserProcessor()
{
}

bool UserProcessor::RegisterPacket()
{
	REGIST_PACKET(m_packetFunc.emplace(std::make_pair(protocol_svr::MESSAGE_LOGOUT_REQ, &UserProcessor::LOGOUT_REQ)));
	REGIST_PACKET(m_packetFunc.emplace(std::make_pair(protocol_svr::MESSAGE_ENTER_MAP_REQ, &UserProcessor::ENTER_MAP_REQ)));
	REGIST_PACKET(m_packetFunc.emplace(std::make_pair(protocol_svr::MESSAGE_ENTITY_MOVE_VELOCITY_REQ, &UserProcessor::ENTITY_MOVE_VELOCITY_REQ)));
	REGIST_PACKET(m_packetFunc.emplace(std::make_pair(protocol_svr::MESSAGE_ENTITY_MOVE_STOP_REQ, &UserProcessor::ENTITY_MOVE_STOP_REQ)));
	
	return true;
}

bool UserProcessor::Create()
{
	RegisterPacket();

	if(false == MESSAGE_PROCESSOR().SetReceiver(MessageProcessor::MESSAGE_TYPE::USER, this))
	{
		return false;
	}

	m_zoneMgr = new ZoneMgr;
	if(nullptr == m_zoneMgr ||
		false == m_zoneMgr->Create())
	{
		return false;
	}	

	m_zoneMgr->CreateZone(1);

	auto threadProc = [this]() -> void {
		this->OnUpdate();
	};
	return m_thread.ThreadCreateFunc(threadProc);
}

void UserProcessor::Destroy()
{
	m_thread.DestroyThread();

	if(m_zoneMgr)
	{
		m_zoneMgr->Destroy();
		delete m_zoneMgr;
		m_zoneMgr = nullptr;
	}
}

void UserProcessor::OnUpdate()
{
	THREAD_UPDATE_START;

	MESSAGE_PROCESSOR().OnUpdate(MessageProcessor::MESSAGE_TYPE::USER);
	
	//m_userMgr->OnUpdate();

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
	}
}

void UserProcessor::PacketParsing_IOCP(char* buffer)
{
	MessageProcessor::MessageUser* user = (MessageProcessor::MessageUser*)buffer;
	
	char* packetBuffer = user->m_data;
	s2::packet_size_t packetSize = *(s2::packet_size_t*)(&packetBuffer[0]);
	s2::packet_protocol_t packetProtocol = *(s2::packet_protocol_t*)(&packetBuffer[2]);

	const char* body = &packetBuffer[4];
	auto iter = m_packetFunc.find(static_cast<protocol_svr::MESSAGE>(packetProtocol));
	if(iter == m_packetFunc.end())
	{
		return;
	}
	if(false == (this->*(iter->second))(user->m_session, body, packetSize - 4))
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

bool UserProcessor::SendPacket_DB(protocol_svr::MESSAGE message, flatbuffers::FlatBufferBuilder& fbb)
{
	return MESSAGE_PROCESSOR().SnedPacket_DB(0, 0, message, fbb);
}