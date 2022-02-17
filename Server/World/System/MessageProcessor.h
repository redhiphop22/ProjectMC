#pragma once

class UserProcessor;
class DataBaseMgr;
class IocpSession;

#define MESSAGE_BUFFER_SIZE	2048

class MessageProcessor : public s2::S2Singleton<MessageProcessor>
{	
public:
	struct MessageUser
	{
		IocpSession* m_session;
		char m_data[MESSAGE_BUFFER_SIZE];

		void SetData(IocpSession* session, const char* data, int32_t size)
		{
			m_session = session;
			memcpy(m_data, data, size);
		}
	};
	struct MessageBase
	{
		char m_data[MESSAGE_BUFFER_SIZE];
	};
	union MessageInfo
	{
		MessageBase		m_base;
		MessageUser		m_user;
	};
public:
	enum class MESSAGE_TYPE
	{
		USER,
		DATABASE,
		ZONE,
	};
	enum class MESSAGE_GROUP_USER
	{
		IOCP,
		DATABASE,
		ZONE,
	};
	using message_processor_t = s2::S2MessageProcessor<MessageInfo>;
	using message_processor_list_t = std::unordered_map<MESSAGE_TYPE, message_processor_t>;


public:
	MessageProcessor();
	~MessageProcessor();

	bool					Create();
	void					Destroy();

	bool					OnUpdate(MESSAGE_TYPE type);

	bool					SetReceiver(MESSAGE_TYPE type, s2::S2MessageReceiver* receiver);
	bool					AddSender(MESSAGE_TYPE type, int32_t groupIdx, int32_t processCount, int32_t bufferCount);

	bool					SnedPacket_User(int32_t groupIdx, int32_t senderIdx, IocpSession* session, const char* packet, int32_t size);
	bool					SnedPacket_User(int32_t groupIdx, int32_t senderIdx, protocol_svr::MESSAGE protocol, flatbuffers::FlatBufferBuilder& fbb);
	bool					SnedPacket_User(int32_t groupIdx, int32_t senderIdx, char* buffer, int32_t size);

	bool					SnedPacket_DB(int32_t groupIdx, int32_t senderIdx, protocol_svr::MESSAGE protocol, flatbuffers::FlatBufferBuilder& fbb);

	bool					SnedPacket_Zone(int32_t groupIdx, int32_t senderIdx, protocol_svr::MESSAGE protocol, flatbuffers::FlatBufferBuilder& fbb);

public:

	message_processor_list_t	m_messageList;
};

#define MESSAGE_PROCESSOR()		MessageProcessor::GetInstance()