#pragma once

#define TCP_DISCONNECT_TYPE_OTHER			0X80000001
#define TCP_DISCONNECT_TYPE_READ_ERROR		0X80000002

class ControllerBase;
class MessageProcessor;

class NetworkTCP
{
public:
	NetworkTCP(MessageProcessor* message);
	~NetworkTCP();

	bool					Create();
	bool					Connect(uint32_t ip, uint16_t port);
	void					DisConnect();

	int32_t					SelectEvent();

	int32_t					PacketParsing(char* buffer, int32_t bufferSize);

	int32_t					SendPacket(protocol::MESSAGE message, flatbuffers::FlatBufferBuilder& fbb);
	int32_t					SendPacket(char* buffer, int32_t size);

protected:
	char *					_GetRecvBuffer();
	int32_t					_OnReceive();
	int32_t					_SendPacketMessage(const char* buffer, int32_t size );

protected:
	SOCKET					m_socket = INVALID_SOCKET;
	WSAEVENT				m_recvEvent = INVALID_HANDLE_VALUE;
	
	int32_t					m_receivedPacketSize = 0;
	char					m_receiveBuffer[CLIENT_SOCKET_BUFFER_SIZE];
	char					m_sendBuffer[CLIENT_SOCKET_BUFFER_SIZE];

	MessageProcessor*		m_messageProcessor;
};
