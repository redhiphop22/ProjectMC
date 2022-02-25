#pragma once

#define TCP_DISCONNECT_TYPE_OTHER			0X80000001
#define TCP_DISCONNECT_TYPE_READ_ERROR		0X80000002

namespace s2 {

class S2SocketConnector
{
public:
	S2SocketConnector();
	virtual ~S2SocketConnector();

	void					SetAddress(uint32_t ip, uint16_t port);
	
	bool					Connect();
	void					DisConnect();

	virtual int32_t			PacketParsing(char* buffer, int32_t size);
	virtual int32_t			SendPacketMessage(const char* buffer, int32_t size);

	int32_t					SelectEvent();
	bool					IsConnected() const;

	

protected:
	virtual int32_t			_OnReceive();

protected:
	const static uint16_t SEND_BUFFER_MAX = 8096;	// �ֱ� MTU 9000
	const static uint16_t RECV_BUFFER_MAX = 8096;

	SOCKET					m_socket = INVALID_SOCKET;
	WSAEVENT				m_recvEvent = INVALID_HANDLE_VALUE;

	uint32_t				m_ip;
	uint16_t				m_port;

	int32_t					m_recvPackSize = 0;
	char					m_recvBuffer[RECV_BUFFER_MAX];

	int32_t					m_sendPackSize = 0;
	char					m_sendBuffer[RECV_BUFFER_MAX];

	//time_t					m_dt32LastSendTime;
	//time_t					m_dt32LastRecvTime;
};
}