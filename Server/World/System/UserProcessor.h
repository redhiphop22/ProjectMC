#pragma once

class IocpSession;
class UserMgr;

class UserProcessor : public s2::S2MessageReceiver
{
	using sesion_func_t = bool(UserProcessor::*)(IocpSession* session, const char* buffer, int32_t size);
	using sesion_list_t = std::unordered_map<protocol::MESSAGE, sesion_func_t>;

	using packet_func_t = bool(UserProcessor::*)(const char* buffer, int32_t size);
	using packet_list_t = std::unordered_map<protocol_svr::MESSAGE, packet_func_t>;

public:
	UserProcessor();
	~UserProcessor();

	bool					Create();
	void					Destroy();
	void					OnUpdate();

	virtual bool			OnMessageUpdate(int32_t groupIdx, void* data) override;

protected:
	bool					SendPacket_DB(protocol_svr::MESSAGE protocol, flatbuffers::FlatBufferBuilder& fbb);
	bool					SendPacket_Zone(protocol_svr::MESSAGE message, flatbuffers::FlatBufferBuilder& fbb);

	void					PacketParsing(int32_t groupIdx, char* buffer);
	void					PacketParsing_IOCP(char* buffer);
	void					PacketParsing_Database(char* buffer);
	void					PacketParsing_Zone(char* buffer);

protected:
	bool					RegisterPacket();
	bool					RegisterPacket_Packet();
	bool					RegisterPacket_Zone();

protected:
	bool					SERVER_CONNECT_C2S(IocpSession* session, const char* buffer, int32_t size);
	bool					ENTER_LOBBY_C2S(IocpSession* session, const char* buffer, int32_t size);
	bool					CHARACTER_INFO_C2S(IocpSession* session, const char* buffer, int32_t size);
	bool					CHARACTER_NAME_DUPLICATION_C2S(IocpSession* session, const char* buffer, int32_t size);
	bool					CHARACTER_CREATE_C2S(IocpSession* session, const char* buffer, int32_t size);
	bool					ENTER_MAP_C2S(IocpSession* session, const char* buffer, int32_t size);
	bool					ENTITY_MOVE_VELOCITY_C2S(IocpSession* session, const char* buffer, int32_t size);
	bool					ENTITY_MOVE_STOP_C2S(IocpSession* session, const char* buffer, int32_t size);

protected:
	bool					SERVER_CONNECT_DB_ACK(const char* buffer, int32_t size);
	bool					CHARACTER_INFO_DB_ACK(const char* buffer, int32_t size);
	bool					CHARACTER_NAME_DUPLICATION_DB_ACK(const char* buffer, int32_t size);
	bool					CHARACTER_CREATE_DB_ACK(const char* buffer, int32_t size);

protected:
	bool					ENTER_MAP_ACK(const char* buffer, int32_t size);
	bool					ENTITY_SPAWN_ACK(const char* buffer, int32_t size);
	bool					ENTITY_DESTROY_ACK(const char* buffer, int32_t size);

private:
	s2::S2Thread			m_thread;

	UserMgr*				m_userMgr;
	sesion_list_t			m_packetFunc;
	packet_list_t			m_dbFunc;
	packet_list_t			m_zoneFunc;
};