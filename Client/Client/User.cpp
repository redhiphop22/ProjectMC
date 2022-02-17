#include "pch.h"
#include "User.h"

#include "MessageProcessor.h"

User::User(MessageProcessor* message)
	: NetworkTCP(message)
{
}

User::~User()
{
	DisConnect();
}

bool User::Create(uint32_t ip, uint16_t port, const uid_t uid, const std::string& id)
{
	m_ip = ip;
	m_port = port;
	m_uid = uid;
	m_id = id;
	return true;
}

void User::OnUpdate()
{
	if(STATE::DISCONNECT == m_state &&
		m_nextConnectTime < GetTickCount64())
	{
		flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
		auto body = protocol::CreateCONNECT_RESULT_C2S(fbb);
		fbb.Finish(body);

		m_messageProcessor->PushData(this, protocol::MESSAGE_CONNECT_RESULT_C2S, (char*)fbb.GetBufferPointer(), fbb.GetSize());
		m_state = STATE::CONNECT;
	}
	else
	{
		SelectEvent();
	}
}

bool User::Connect()
{
	if(false == NetworkTCP::Connect(m_ip, m_port))
	{
		DisConnect();
		return false;
	}
	m_state = STATE::LOBBY;
	return true;
}

void User::DisConnect()
{
	NetworkTCP::DisConnect();
	m_state = STATE::DISCONNECT;
	m_nextConnectTime = GetTickCount64() + 3000;
}

void User::MakeCHARACTER_CREATE_C2S(flatbuffers::FlatBufferBuilder& fbb)
{
	common::CHARACTER_TYPE type = static_cast<common::CHARACTER_TYPE>((rand() % (common::CHARACTER_TYPE_MAX)) + 1);
	uint16_t belong = rand();
	auto face = common::CreateCHARACTER_FACE(fbb,
		static_cast<uint16_t>(rand()),		// Çì¾î
		static_cast<uint8_t>(rand()),
		static_cast<uint16_t>(rand()),		// ¾ó±¼
		static_cast<uint8_t>(rand()),
		static_cast<uint16_t>(rand()),		// ´«µ¿ÀÚ
		static_cast<uint8_t>(rand()),
		static_cast<uint16_t>(rand()),		// ´«½ç
		static_cast<uint8_t>(rand()),
		static_cast<uint16_t>(rand()),		// ÄÚ
		static_cast<uint8_t>(rand()),
		static_cast<uint16_t>(rand()),		// ÀÔ¼ú
		static_cast<uint8_t>(rand())
		);
	std::vector<flatbuffers::Offset<common::CHARACTER_EQUIPMENT>> equipmentList;
	for(int32_t i = 0 ; i < common::CHARACTER_EQUIPMENT_TYPE_MAX+1 ; ++i)
	{
		equipmentList.push_back(
			common::CreateCHARACTER_EQUIPMENT(fbb,
				static_cast<common::CHARACTER_EQUIPMENT_TYPE>(i),
				rand()));
	}
		
	auto body = protocol::CreateCHARACTER_CREATE_C2S(fbb,
		fbb.CreateString(m_nick.c_str()),
		type,
		belong,
		face,
		fbb.CreateVector(equipmentList)
	);
	fbb.Finish(body);

	
}