#include "pch.h"
#include "ControllerBase.h"
#include "User.h"

ControllerBase::ControllerBase()
{
}

ControllerBase::~ControllerBase()
{
}

bool ControllerBase::RegistPacket()
{
	REGIST_PACKET(m_packetFunc.emplace(std::make_pair(protocol::MESSAGE_CONNECT_RESULT_C2S, &ControllerBase::CONNECT_RESULT_C2S)));
	REGIST_PACKET(m_packetFunc.emplace(std::make_pair(protocol::MESSAGE_CONNECT_RESULT_S2C, &ControllerBase::CONNECT_RESULT_S2C)));
	REGIST_PACKET(m_packetFunc.emplace(std::make_pair(protocol::MESSAGE_SERVER_CONNECT_S2C, &ControllerBase::SERVER_CONNECT_S2C)));
	REGIST_PACKET(m_packetFunc.emplace(std::make_pair(protocol::MESSAGE_ENTER_LOBBY_S2C, &ControllerBase::ENTER_LOBBY_S2C)));
	REGIST_PACKET(m_packetFunc.emplace(std::make_pair(protocol::MESSAGE_CHARACTER_INFO_S2C, &ControllerBase::CHARACTER_INFO_S2C)));
	REGIST_PACKET(m_packetFunc.emplace(std::make_pair(protocol::MESSAGE_CHARACTER_NAME_DUPLICATION_S2C, &ControllerBase::CHARACTER_NAME_DUPLICATION_S2C)));
	REGIST_PACKET(m_packetFunc.emplace(std::make_pair(protocol::MESSAGE_CHARACTER_CREATE_S2C, &ControllerBase::CHARACTER_CREATE_S2C)));
	REGIST_PACKET(m_packetFunc.emplace(std::make_pair(protocol::MESSAGE_ENTER_MAP_S2C, &ControllerBase::ENTER_MAP_S2C)));
	REGIST_PACKET(m_packetFunc.emplace(std::make_pair(protocol::MESSAGE_ENTITY_SPAWN_S2C, &ControllerBase::ENTITY_SPAWN_S2C)));
	REGIST_PACKET(m_packetFunc.emplace(std::make_pair(protocol::MESSAGE_ENTITY_DESTROY_S2C, &ControllerBase::ENTITY_DESTROY_S2C)));	

	return true;
}

bool ControllerBase::Create()
{
	RegistPacket();

	return true;
}

void ControllerBase::Destroy()
{
}

bool ControllerBase::PacketParsing(User* user, packet_protocol_t protocol, char* buffer, int32_t size)
{
	auto iter = m_packetFunc.find(static_cast<protocol::MESSAGE>(protocol));
	if(iter == m_packetFunc.end())
		return false;

	(this->*(iter->second))(user, buffer, size);

	return true;
}

void ControllerBase::CONNECT_RESULT_C2S(User* user, const char* buffer, int32_t size)
{
	user->Connect();
}

void ControllerBase::LOGIN_C2S(User* user, flatbuffers::FlatBufferBuilder& fbb)
{
	user->SendPacket(protocol::MESSAGE_LOGIN_C2S, fbb);
}

void ControllerBase::SERVER_CONNECT_C2S(User* user, flatbuffers::FlatBufferBuilder& fbb)
{
	user->SendPacket(protocol::MESSAGE_SERVER_CONNECT_C2S, fbb);
}

void ControllerBase::ENTER_LOBBY_C2S(User* user, flatbuffers::FlatBufferBuilder& fbb)
{
	user->SendPacket(protocol::MESSAGE_ENTER_LOBBY_C2S, fbb);
}

void ControllerBase::CHARACTER_INFO_C2S(User* user, flatbuffers::FlatBufferBuilder& fbb)
{
	user->SendPacket(protocol::MESSAGE_CHARACTER_INFO_C2S, fbb);
}

void ControllerBase::CHARACTER_NAME_DUPLICATION_C2S(User* user, flatbuffers::FlatBufferBuilder& fbb)
{
	user->SendPacket(protocol::MESSAGE_CHARACTER_NAME_DUPLICATION_C2S, fbb);
}

void ControllerBase::CHARACTER_CREATE_C2S(User* user, flatbuffers::FlatBufferBuilder& fbb)
{
	user->SendPacket(protocol::MESSAGE_CHARACTER_CREATE_C2S, fbb);
}

void ControllerBase::ENTER_MAP_C2S(User* user, flatbuffers::FlatBufferBuilder& fbb)
{
	user->SendPacket(protocol::MESSAGE_ENTER_MAP_C2S, fbb);
}