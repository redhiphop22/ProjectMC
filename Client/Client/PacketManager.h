#pragma once

#include <unordered_map>

class CClientApp;
class NetworkTCP;
class ControllerBase;

#define PACKET_CONVERT2(type, buf, len) \
	flatbuffers::Verifier verifier((const uint8_t*)buf, len); \
	if(false == verifier.VerifyBuffer<protocol::type>()) \
		return false; \
	auto ack = flatbuffers::GetRoot<protocol::type>(buffer);

class PacketManager
{
	using packet_func_t = bool(PacketManager::*)(const char* buffer, int32_t size);
	using packet_list_t = std::unordered_map<packet_protocol_t, packet_func_t>;

public:
	PacketManager(ControllerBase* controller, NetworkTCP* TCP);
	~PacketManager();

	bool					Create();
	void					Destroy();
	bool					PacketParsing(packet_protocol_t protocol, const char* buffer, int32_t size);

public:
	bool					ConnectServerReq(uint32_t ip, uint16_t port);

	bool					CONNECT_RESULT_S2C(const char* buffer, int32_t size);
	bool					SERVER_CONNECT_S2C(const char* buffer, int32_t size);

private:
	CClientApp*				m_app = nullptr;
	NetworkTCP*				m_TCP = nullptr;
	ControllerBase*			m_controller = nullptr;

	packet_list_t			m_packetFunc;

};