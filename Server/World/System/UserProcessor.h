#pragma once

class IocpSession;
class UserMgr;

class UserProcessor : public s2::S2Thread, s2::S2MessageReceiver
{
	using sesion_func_t = bool(UserProcessor::*)(IocpSession* session, const char* buffer, int32_t size);
	using packet_func_t = bool(UserProcessor::*)(const char* buffer, int32_t size);

public:
	UserProcessor();
	~UserProcessor();

	bool					Create(int32_t workerCount, int32_t dbCount, int32_t bufferCount);
	void					Destroy();
	void					OnUpdate();

	virtual bool			OnThreadUpdate() override;
	virtual bool			OnMessageUpdate(int32_t groupIdx, void* data) override;

protected:
	bool					SendPacket_DB(protocol_svr::MESSAGE message, flatbuffers::FlatBufferBuilder& fbb);

	void					PacketParsing(int32_t groupIdx, char* buffer);

protected:
	bool					RegisterPacket();

protected:
	bool					SERVER_CONNECT_C2S(IocpSession* session, const char* buffer, int32_t size);
	bool					CHARACTER_INFO_C2S(IocpSession* session, const char* buffer, int32_t size);
	bool					CHARACTER_NAME_DUPLICATION_C2S(IocpSession* session, const char* buffer, int32_t size);
	bool					CHARACTER_CREATE_C2S(IocpSession* session, const char* buffer, int32_t size);
	bool					MAP_ENTER_C2S(IocpSession* session, const char* buffer, int32_t size);

protected:
	bool					SERVER_CONNECT_DB_ACK(const char* buffer, int32_t size);
	bool					CHARACTER_INFO_DB_ACK(const char* buffer, int32_t size);
	bool					CHARACTER_NAME_DUPLICATION_DB_ACK(const char* buffer, int32_t size);
	bool					CHARACTER_CREATE_DB_ACK(const char* buffer, int32_t size);

public:
	UserMgr*				m_userMgr;
	std::unordered_map<protocol::MESSAGE, sesion_func_t> m_packetFunc;
	std::unordered_map<protocol_svr::MESSAGE, packet_func_t> m_dbFunc;
};