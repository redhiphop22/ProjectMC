#include "S2Engine.h"
#include "S2SocketConnector.h"

namespace s2 {

S2SocketConnector::S2SocketConnector()
{
}

S2SocketConnector::~S2SocketConnector()
{
}

void S2SocketConnector::SetAddress(uint32_t ip, uint16_t port)
{
	m_ip = htonl(ip);
	m_port = ::htons(port);
}

bool S2SocketConnector::Connect()
{
	// ���ú� �̺�Ʈ ����
	m_recvEvent = ::WSACreateEvent();

	// ���� ����
	m_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(INVALID_SOCKET == m_socket)
	{
		S2TRACE(_T("[S2SocketConnector::OnConnectTo] Error Socket Is Invalid \n"));
		DisConnect();
		return false;
	}

	// ���� ����
	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family		= AF_INET;
	serverAddr.sin_addr.s_addr	= m_ip;
	serverAddr.sin_port			= m_port;

	int32_t result = ::connect(m_socket, (sockaddr*)&serverAddr, sizeof(serverAddr));
	if(SOCKET_ERROR == result)
	{
		DWORD error = ::GetLastError();
		DisConnect();
		return false;
	}

	// ���ú� ���� ��� �ʱ�ȭ
	m_recvPackSize = 0;

	return true;
}

void S2SocketConnector::DisConnect()
{
	if (INVALID_HANDLE_VALUE != m_recvEvent)
	{
		::WSACloseEvent(m_recvEvent);
		m_recvEvent = INVALID_HANDLE_VALUE;
	}

	if (INVALID_SOCKET != m_socket)
	{
		::shutdown(m_socket, SD_BOTH);
		::closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}

	m_recvPackSize = 0;
}

int32_t S2SocketConnector::_OnReceive()
{
	//m_dt32LastRecvTime = S2Time::StandTime();

	// ��Ŷ�� �޴´�.
	int32_t recvdByte = ::recv(m_socket, m_recvBuffer + m_recvPackSize, S2_PACKET_BUFFER_SIZE, 0);

	if(SOCKET_ERROR == recvdByte)
	{
		int32_t error = ::WSAGetLastError();
		S2LOG_ERROR("[S2SocketConnector::_OnReceive] lastError = %d", error);

		DisConnect();
		return TCP_DISCONNECT_TYPE_READ_ERROR;
	}
	if (0 == recvdByte)
	{
		return recvdByte;	// ClosedSocket
	}

	// ���� ������ ũ�� ����
	m_recvPackSize += recvdByte;

	if(RECV_BUFFER_MAX < m_recvPackSize)
	{
		S2LOG_ERROR("Buffer overflow(%d bytes)", m_recvPackSize);
		DisConnect();
		return TCP_DISCONNECT_TYPE_READ_ERROR;
	}

	int32_t recvTotalSize = m_recvPackSize;
	int32_t readSize = 0;
	packet_size_t packetSize;

	auto checkPacket = [this, &packetSize, &recvTotalSize, &readSize]() -> bool {
		if (recvTotalSize < PACKET_HEAD_SIZE)
			return false;

		packetSize = *((packet_size_t*)&m_recvBuffer[readSize]);
		if (recvTotalSize < packetSize)
			return false;

		return true;
	};	

	while(checkPacket())
	{
		PacketParsing(&m_recvBuffer[readSize], packetSize);

		recvTotalSize -= packetSize;
		readSize += packetSize;
	}

	if(0 < readSize && 0 < recvTotalSize)
	{
		memmove(m_recvBuffer, &m_recvBuffer[readSize], recvTotalSize);
	}
	m_recvPackSize = recvTotalSize;

	return recvdByte;
}

int32_t S2SocketConnector::PacketParsing(char* buffer, int32_t size)
{
	// Do nothing
	return 0;
}

int32_t S2SocketConnector::SendPacketMessage(const char* buffer, int32_t size)
{
	// ������ �������� ������ �ʴ´�.
	if (INVALID_SOCKET == m_socket)	return 0;

	// ��Ŷ�� �� ���������� �ݺ��Ѵ�.
	int32_t sendIdx = 0;
	while (size > sendIdx)
	{
		int32_t sendedByte = ::send(m_socket, buffer + sendIdx, size - sendIdx, 0);

		// ���� ���� 
		if (SOCKET_ERROR == sendedByte)
		{
			int32_t error = ::WSAGetLastError();
			S2LOG_ERROR("[S2SocketConnector::SendPacketMessage] lastError = %d", error);
			//i3Net::GetError(message);

			return -1;
		}

		// ������ ��������.
		if (0 == sendedByte)
		{
			return 0;
		}

		// ���� ����Ʈ��ŭ �ε��� ����
		sendIdx += sendedByte;
	}

	//m_dt32LastSendTime = S2Time::StandTime();

	return sendIdx;
}

int32_t S2SocketConnector::SelectEvent()
{
	int32_t result = 0;
	WSANETWORKEVENTS event;
	memset(&event, 0, sizeof(event));

	// ó���� �̺�Ʈ�� Ȯ���Ѵ�.
	if (SOCKET_ERROR != ::WSAEventSelect(m_socket, m_recvEvent, FD_WRITE | FD_READ | FD_CLOSE))
	{
		if (SOCKET_ERROR != ::WSAEnumNetworkEvents(m_socket, m_recvEvent, &event))
		{
			if (event.lNetworkEvents & FD_READ)
			{
				result = _OnReceive();
			}
			if (event.lNetworkEvents & FD_CLOSE)
			{
				DisConnect();
				result = TCP_DISCONNECT_TYPE_OTHER;
			}

			return result;
		}
	}

	// ���� �߻��� ó���Ѵ�.
	// ���� �߻� ������ �����ϱ⶧���� ����� �������� �ʽ��ϴ�.
	int32_t error = ::WSAGetLastError();
	S2LOG_ERROR("[S2SocketConnector::SelectEvent] lastError = %d", error);
	//i3Net::GetError(message);

	return result;
}

bool S2SocketConnector::IsConnected() const
{
	if (INVALID_SOCKET == m_socket)
	{
		return false;
	}

	return true;
}


}