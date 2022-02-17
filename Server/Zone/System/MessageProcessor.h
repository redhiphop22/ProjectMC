#pragma once

class UserProcessor;
class DataBaseMgr;
class IocpSession;

class MessageProcessor : public s2::S2Singleton<MessageProcessor>
{	
public:
	struct MessageInfo_User
	{
		IocpSession* m_session;
		char m_data[4096];

		void SetData(IocpSession* session, const char* data, int32_t size)
		{
			m_session = session;
			memcpy(m_data, data, size);
		}
	};

	struct MessageInfo_DB
	{
		char m_data[4096];
	};
public:
	enum class MESSAGE_TYPE
	{
		USER,
		DATABASE,
	};
	enum class MESSAGE_GROUP_USER
	{
		NETWORK,
		DATABASE,
	};

	using message_processor_user_t = s2::S2MessageProcessor<MessageInfo_User>;
	using message_processor_db_t = s2::S2MessageProcessor<MessageInfo_DB>;

public:
	MessageProcessor();
	~MessageProcessor();

	bool					Create();
	void					Destroy();

	void					OnUpdate(MESSAGE_TYPE type);

	bool					SetReceiver(MESSAGE_TYPE type, s2::S2MessageReceiver* receiver);
	bool					AddSender(MESSAGE_TYPE type, int32_t groupIdx, int32_t processIdx, int32_t bufferCount);

	bool					SnedPacket_User(int32_t groupIdx, int32_t senderIdx, IocpSession* session, const char* packet, int32_t size);
	bool					SnedPacket_User(int32_t groupIdx, int32_t senderIdx, protocol_svr::MESSAGE message, flatbuffers::FlatBufferBuilder& fbb);

	bool					SnedPacket_DB(int32_t groupIdx, int32_t senderIdx, protocol_svr::MESSAGE message, flatbuffers::FlatBufferBuilder& fbb);

public:

	message_processor_user_t	m_messageUser;
	message_processor_db_t		m_messageDB;
};

#define MESSAGE_PROCESSOR()		MessageProcessor::GetInstance()