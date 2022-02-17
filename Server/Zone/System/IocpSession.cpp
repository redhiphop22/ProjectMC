#include "Zone.h"
#include "IocpSession.h"
//#include "UserProcessor.h"

IocpSession::IocpSession(int32_t idx)
	: s2::S2IocpSession(idx)
{
}

IocpSession::~IocpSession()
{
}

void IocpSession::CompletionLogin(uid_t UID)
{
	m_UID = UID;
	OnCompletionLogin();
}

bool IocpSession::OnAccept()
{
	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
	auto body = protocol_svr::CreateCONNECT_RESULT_ACK(fbb, common::RESULT_CODE_SUCCESS);
	fbb.Finish(body);
	SendPacket(protocol_svr::MESSAGE_CONNECT_RESULT_ACK, fbb);

	return true;
}

bool IocpSession::OnClose()
{
	S2IocpSession::Destroy();

	return true;
}

bool IocpSession::OnRecvPacket(int32_t idx, const char* packet, int32_t size)
{
	return MESSAGE_PROCESSOR().SnedPacket_User(static_cast<int32_t>(MessageProcessor::MESSAGE_GROUP_USER::IOCP), idx, this, packet, size);
}

bool IocpSession::SendPacket(protocol_svr::MESSAGE messageID, flatbuffers::FlatBufferBuilder& fbb)
{
	uint8_t* buffer = fbb.GetBufferPointer();
	int32_t size = fbb.GetSize();

	s2::packet_size_t* packetSize = (s2::packet_size_t*)&m_sendBuffer[0];
	s2::packet_protocol_t* packetProtocol = (s2::packet_protocol_t*)&m_sendBuffer[2];

	*packetSize = size + PACKET_HEAD_SIZE;
	*packetProtocol = messageID;

	memcpy(&m_sendBuffer[4], buffer, size);
	
	return OnSendPacket(m_sendBuffer, *packetSize);
}