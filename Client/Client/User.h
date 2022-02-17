#pragma once

#include "NetworkTCP.h"

class User : public NetworkTCP
{
public:
	enum class STATE
	{
		CONNECT,
		LOBBY,
		DISCONNECT,
	};

public:
	User(MessageProcessor* message);
	~User();

	bool					Create(uint32_t ip, uint16_t port, const uid_t uid, const std::string& id);

	void					OnUpdate();

	bool					Connect();
	void					DisConnect();

	STATE					GetState()			{	return m_state;	}

	void					MakeCHARACTER_CREATE_C2S(flatbuffers::FlatBufferBuilder& fbb);

public:
	uid_t					m_uid = 0;
	std::string				m_id;

	std::string				m_nick;

private:
	uint32_t				m_ip = 0;
	uint16_t				m_port = 0;
	STATE					m_state = STATE::DISCONNECT;
	time_t					m_nextConnectTime = 0;
};