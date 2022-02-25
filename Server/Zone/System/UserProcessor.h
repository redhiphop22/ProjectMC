#pragma once

class IocpSession;
class ZoneMgr;

class UserProcessor : public s2::S2MessageReceiver
{
	using sesion_func_t = bool(UserProcessor::*)(IocpSession* session, const char* buffer, int32_t size);
	using sesion_list_t = std::unordered_map<protocol_svr::MESSAGE, sesion_func_t>;

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

	void					PacketParsing(int32_t groupIdx, char* buffer);
	void					PacketParsing_IOCP(char* buffer);
	void					PacketParsing_Database(char* buffer);

protected:
	bool					RegisterPacket();

protected:
	bool					LOGOUT_REQ(IocpSession* session, const char* buffer, int32_t size);
	bool					ENTER_MAP_REQ(IocpSession* session, const char* buffer, int32_t size);
	bool					ENTITY_MOVE_VELOCITY_REQ(IocpSession* session, const char* buffer, int32_t size);
	bool					ENTITY_MOVE_STOP_REQ(IocpSession* session, const char* buffer, int32_t size);
	
private:
	s2::S2Thread			m_thread;

	ZoneMgr*				m_zoneMgr;
	sesion_list_t			m_packetFunc;
	packet_list_t			m_dbFunc;
};