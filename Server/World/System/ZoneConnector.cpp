#include "World.h"
#include "ZoneConnector.h"

ZoneConnector::ZoneConnector()
{
}

ZoneConnector::~ZoneConnector()
{
}

bool ZoneConnector::RegisterPacket()
{
	REGIST_PACKET(m_recvFunc.emplace(std::make_pair(protocol_svr::MESSAGE_CONNECT_RESULT_ACK, &ZoneConnector::CONNECT_RESULT_ACK)));

	return true;
}

bool ZoneConnector::Create(uint32_t ip, uint16_t port)
{
	RegisterPacket();

	if(false == MESSAGE_PROCESSOR().SetReceiver(MessageProcessor::MESSAGE_TYPE::ZONE, this))
	{
		return false;
	}	

	SetAddress(ip, port);

	S2SocketConnector::Connect();

	auto threadProc = [this]() -> void {
		this->OnUpdate();
	};
	return m_thread.ThreadCreateFunc(threadProc);
}

void ZoneConnector::Destroy()
{
	m_thread.DestroyThread();

	S2SocketConnector::DisConnect();
}

bool ZoneConnector::SendPacket(protocol_svr::MESSAGE protocol, flatbuffers::FlatBufferBuilder& fbb)
{
	uint8_t* buffer = fbb.GetBufferPointer();
	int32_t size = fbb.GetSize();

	s2::packet_size_t* packetSize = (s2::packet_size_t*)&m_sendBuffer[0];
	s2::packet_protocol_t* packetProtocol = (s2::packet_protocol_t*)&m_sendBuffer[2];

	*packetSize = size + PACKET_HEAD_SIZE;
	*packetProtocol = protocol;

	memcpy(&m_sendBuffer[4], buffer, size);
	
	return SendPacketMessage(m_sendBuffer, *packetSize);
}

void ZoneConnector::OnUpdate()
{
	THREAD_UPDATE_START;

	MESSAGE_PROCESSOR().OnUpdate(MessageProcessor::MESSAGE_TYPE::ZONE);

	if(false == S2SocketConnector::IsConnected())
	{
		if(false == S2SocketConnector::Connect())
		{	
			return true;
		}
	}

	if(S2SocketConnector::SelectEvent())
	{
		isWorking = true;
	}

	THREAD_UPDATE_END;
}

bool ZoneConnector::OnMessageUpdate(int32_t groupIdx, void* data)
{
	MessageProcessor::MessageBase* user = (MessageProcessor::MessageBase*)data;

	char* packetBuffer = user->m_data;
	s2::packet_size_t packetSize = *(s2::packet_size_t*)(&packetBuffer[0]);

	memcpy(m_sendBuffer, packetBuffer, packetSize);
	
	return SendPacketMessage(m_sendBuffer, packetSize);
}

int32_t ZoneConnector::PacketParsing(char* buffer, int32_t size)
{
	MessageProcessor::MessageBase* user = (MessageProcessor::MessageBase*)buffer;
	
	char* packetBuffer = user->m_data;
	s2::packet_size_t packetSize = *(s2::packet_size_t*)(&packetBuffer[0]); \
	s2::packet_protocol_t packetProtocol = *(s2::packet_protocol_t*)(&packetBuffer[2]); \

	const char* body = &packetBuffer[4];
	auto iter = m_recvFunc.find(static_cast<protocol_svr::MESSAGE>(packetProtocol));
	if (iter != m_recvFunc.end())
	{
		if (false == (this->*(iter->second))(body, packetSize - 4))
		{
			return 0;
		}
	}
	else
	{
		MESSAGE_PROCESSOR().SnedPacket_User(static_cast<int32_t>(MessageProcessor::MESSAGE_TYPE::ZONE), 0, buffer, size);
	}
	return 0;
}

bool ZoneConnector::CONNECT_RESULT_ACK(const char* buffer, int32_t size)
{
	return true;
}