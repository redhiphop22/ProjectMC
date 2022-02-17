#pragma once

class MessageThread
{
public:
	struct MessageInfo
	{
		User*				m_user;
		packet_protocol_t	m_protocol;
		char				m_buffer[PACKET_BUFFER_SIZE];
		int32_t				m_size;

		void operator=(const MessageInfo& info)
		{
			m_user = info.m_user;
			m_protocol = info.m_protocol;
			memcpy(m_buffer, info.m_buffer, info.m_size);
			m_size = info.m_size;
		}

		void Set(User* user, packet_protocol_t protocol, char buffer[PACKET_BUFFER_SIZE], int32_t size)
		{
			m_user = user;
			m_protocol = protocol;
			memcpy(m_buffer, buffer, size);
			m_size = size;
		}
	};

	using thread_id_t = DWORD;

public:
	enum class THREAD_STATE
	{
		WATTING,
		RUNNING,
		EXIT,
	};

public:
	MessageThread(ControllerBase* controller);
	~MessageThread();

	bool					Create(bool useThread);
	void					Destroy();

	void					OnThreadExecute();
	void					OnRunning();

	void					SetState(THREAD_STATE state)	{ m_state = state;	}
	THREAD_STATE			GetState()						{ return m_state;	}

	void					PushData(MessageInfo& message);

private:
	HANDLE					m_thread = INVALID_HANDLE_VALUE;
	thread_id_t				m_threadID = 0;
	bool					m_isRunning = false;
	THREAD_STATE			m_state = THREAD_STATE::WATTING;

	MessageInfo				m_message;
	ControllerBase*			m_controller;
	bool					m_useThread;
};

class MessageProcessor
{
public:
	using thread_list_t = std::vector<MessageThread*>;

	using message_list_t = std::deque<MessageThread::MessageInfo>;

public:
	MessageProcessor(ControllerBase* controller);
	~MessageProcessor();

	bool					Create(int32_t workerCount, bool useThread);
	void					Destroy();
	void					OnUpdate();

	void					PushData(User* user, packet_protocol_t protocol, char* buffer, int32_t size);

	MessageThread*			GetWaitThread();

private:
	thread_list_t			m_threadList;
	message_list_t			m_packetList;
	ControllerBase*			m_controller;
};
