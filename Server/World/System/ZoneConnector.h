#pragma once

#include <Winsock2.h>

class ZoneConnector : public s2::S2SocketConnector, s2::S2MessageReceiver
{
	using packet_func_t = bool(ZoneConnector::*)(const char* buffer, int32_t size);
	using packet_list_t = std::unordered_map<protocol_svr::MESSAGE, packet_func_t>;

public:
	ZoneConnector();
	~ZoneConnector();

	bool					Create(uint32_t ip, uint16_t port);
	void					Destroy();
	void					OnUpdate();

	bool					RegisterPacket();

	bool					SendPacket(protocol_svr::MESSAGE protocol, flatbuffers::FlatBufferBuilder& fbb);

	virtual bool			OnMessageUpdate(int32_t groupIdx, void* data) override;
	virtual int32_t			PacketParsing(char* buffer, int32_t size) override;

public:
	bool					CONNECT_RESULT_ACK(const char* buffer, int32_t size);
	bool					ENTER_MAP_ACK(const char* buffer, int32_t size);

	bool					LOGOUT_REQ(const char* buffer, int32_t size);
	bool					ENTER_MAP_REQ(const char* buffer, int32_t size);

private:
	s2::S2Thread			m_thread;
	packet_list_t			m_recvFunc;
	packet_list_t			m_sendFunc;
};