#include "pch.h"
#include "PacketManager.h"
#include "NetworkTCP.h"
#include "ControllerBase.h"

PacketManager::PacketManager(ControllerBase* controller, NetworkTCP* TCP)
	: m_controller(controller)
	, m_TCP(TCP)
{
	m_TCP->SetPacketMgr(this);
}

PacketManager::~PacketManager()
{
}

bool PacketManager::Create()
{
	m_packetFunc.emplace(std::make_pair(protocol::MESSAGE_CONNECT_RESULT_S2C, &PacketManager::CONNECT_RESULT_S2C));
	m_packetFunc.emplace(std::make_pair(protocol::MESSAGE_SERVER_CONNECT_S2C, &PacketManager::SERVER_CONNECT_S2C));
	
	return true;
}

void PacketManager::Destroy()
{
}

bool PacketManager::PacketParsing(packet_protocol_t protocol, const char* buffer, int32_t size)
{
	auto iter = m_packetFunc.find(protocol);
	if(iter == m_packetFunc.end())
		return false;

	if(false == (this->*(iter->second))(buffer, size))
		return false;

	return true;
}

bool PacketManager::ConnectServerReq(uint32_t ip, uint16_t port)
{
	if(false == m_TCP->Create(ip, port ))
	{
		return false;
	}

	return true;
}

bool PacketManager::CONNECT_RESULT_S2C(const char* buffer, int32_t size)
{
	PACKET_CONVERT2(CONNECT_RESULT_S2C, buffer, size);
	if(common::RESULT_CODE_SUCESS == ack->result())
	{
		flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
		auto body = protocol::CreateSERVER_CONNECT_C2S(fbb,
			1
		);
		fbb.Finish(body);

		m_TCP->SendPacket(protocol::MESSAGE::MESSAGE_SERVER_CONNECT_C2S, fbb);

		m_controller->CONNECT_RESULT_S2C();		
	}

	return true;
}

bool PacketManager::SERVER_CONNECT_S2C(const char* buffer, int32_t size)
{
	PACKET_CONVERT2(SERVER_CONNECT_S2C, buffer, size);
	if(common::RESULT_CODE_SUCESS == ack->result())
	{
		m_controller->SERVER_CONNECT_S2C();
	}

	return true;
}