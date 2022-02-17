#include "Zone.h"
#include "MessageProcessor.h"

MessageProcessor::MessageProcessor()
{
}

MessageProcessor::~MessageProcessor()
{
}

bool MessageProcessor::Create()
{
	return true;
}

void MessageProcessor::Destroy()
{
}

void MessageProcessor::OnUpdate(MESSAGE_TYPE type)
{
	switch(type)
	{
	case MESSAGE_TYPE::USER:		m_messageUser.MessageUpdate();		break;
	case MESSAGE_TYPE::DATABASE:	m_messageDB.MessageUpdate();		break;
	}
}

bool MessageProcessor::SetReceiver(MESSAGE_TYPE type, s2::S2MessageReceiver* receiver)
{
	switch(type)
	{
	case MESSAGE_TYPE::USER:		m_messageUser.SetReceiver(receiver);	break;
	case MESSAGE_TYPE::DATABASE:	m_messageDB.SetReceiver(receiver);		break;
	default:
		return false;
	}
	return true;
}

bool MessageProcessor::AddSender(MESSAGE_TYPE type, int32_t groupIdx, int32_t processIdx, int32_t bufferCount)
{
	switch(type)
	{
	case MESSAGE_TYPE::USER:		return m_messageUser.AddSender(groupIdx, processIdx, bufferCount);
	case MESSAGE_TYPE::DATABASE:	return m_messageDB.AddSender(groupIdx, processIdx, bufferCount);
	}
	return false;
}

bool MessageProcessor::SnedPacket_User(int32_t groupIdx, int32_t senderIdx, IocpSession* session, const char* packet, int32_t size)
{
	auto ringBuffer = m_messageUser.GetRingBuffer(groupIdx, senderIdx);
	if(nullptr == ringBuffer)
	{
		return false;
	}
	auto pushData = ringBuffer->GetPushData();
	if(nullptr == pushData)
	{
		return false;
	}
	pushData->SetData(session, packet, size);

	ringBuffer->PushCompleted();

	return true;
}

bool MessageProcessor::SnedPacket_User(int32_t groupIdx, int32_t senderIdx, protocol_svr::MESSAGE message, flatbuffers::FlatBufferBuilder& fbb)
{
	auto ringBuffer = m_messageUser.GetRingBuffer(groupIdx, senderIdx);
	if(nullptr == ringBuffer)
	{
		return false;
	}
	auto pushData = ringBuffer->GetPushData();
	if(nullptr == pushData)
	{
		return false;
	}
	auto* date = pushData->m_data;

	uint16_t packetSize = fbb.GetSize() + 4;
	memcpy(date,		&packetSize,			sizeof(uint16_t));
	memcpy(&date[2],	&message,				sizeof(protocol_svr::MESSAGE));
	memcpy(&date[4],	fbb.GetBufferPointer(),	fbb.GetSize());

	ringBuffer->PushCompleted();

	return true;
}

bool MessageProcessor::SnedPacket_DB(int32_t groupIdx, int32_t senderIdx, protocol_svr::MESSAGE message, flatbuffers::FlatBufferBuilder& fbb)
{
	auto ringBuffer = m_messageDB.GetRingBuffer(groupIdx, senderIdx);
	if(nullptr == ringBuffer)
	{
		return false;
	}
	auto pushData = ringBuffer->GetPushData();
	if(nullptr == pushData)
	{
		return false;
	}
	auto* date = pushData->m_data;

	uint16_t packetSize = fbb.GetSize() + 4;
	memcpy(date,		&packetSize,			sizeof(uint16_t));
	memcpy(&date[2],	&message,				sizeof(protocol_svr::MESSAGE));
	memcpy(&date[4],	fbb.GetBufferPointer(),	fbb.GetSize());

	ringBuffer->PushCompleted();

	return true;
}


