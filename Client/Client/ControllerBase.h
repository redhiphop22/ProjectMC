#pragma once

#define PACKET_CONVERT(type, buf, len) \
	flatbuffers::Verifier verifier((const uint8_t*)buf, len); \
	if(false == verifier.VerifyBuffer<protocol::type>()) \
		return; \
	auto msg = flatbuffers::GetRoot<protocol::type>(buf);

class User;

class ControllerBase
{
protected:
	using packet_func_t = void(ControllerBase::*)(User* user, const char* buffer, int32_t size);
	using packet_list_t = std::unordered_map<protocol::MESSAGE, packet_func_t>;

public:
	ControllerBase();
	virtual ~ControllerBase();

	bool					Create();
	void					Destroy();
	bool					RegistPacket();
	bool					PacketParsing(User* user, packet_protocol_t protocol, char* buffer, int32_t size);

public:
	virtual void			CONNECT_RESULT_C2S(User* user, const char* buffer, int32_t size);

	virtual void			LOGIN_C2S(User* user, flatbuffers::FlatBufferBuilder& fbb);
	virtual void			SERVER_CONNECT_C2S(User* user, flatbuffers::FlatBufferBuilder& fbb);
	virtual void			CHARACTER_INFO_C2S(User* user, flatbuffers::FlatBufferBuilder& fbb);
	virtual void			CHARACTER_NAME_DUPLICATION_C2S(User* user, flatbuffers::FlatBufferBuilder& fbb);
	virtual void			CHARACTER_CREATE_C2S(User* user, flatbuffers::FlatBufferBuilder& fbb);

protected:	
	virtual void			CONNECT_RESULT_S2C(User* user, const char* buffer, int32_t size) {};
	virtual void			SERVER_CONNECT_S2C(User* user, const char* buffer, int32_t size) {};
	virtual void			CHARACTER_INFO_S2C(User* user, const char* buffer, int32_t size) {};
	virtual void			CHARACTER_NAME_DUPLICATION_S2C(User* user, const char* buffer, int32_t size) {};
	virtual void			CHARACTER_CREATE_S2C(User* user, const char* buffer, int32_t size) {};

protected:
	packet_list_t			m_packetFunc;
};