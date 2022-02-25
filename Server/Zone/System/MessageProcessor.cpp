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

bool MessageProcessor::OnUpdate(MESSAGE_TYPE type)
{
	auto message = m_messageList.find(type);
	if(message == m_messageList.end())
	{
		return false;
	}
	return message->second.MessageUpdate();
}

bool MessageProcessor::SetReceiver(MESSAGE_TYPE type, s2::S2MessageReceiver* receiver)
{
	auto message = m_messageList.find(type);
	if(message == m_messageList.end())
	{
		auto result = m_messageList.emplace(std::make_pair(type, message_processor_t()));
		if(false == result.second)
		{
			return false;
		}
		message = result.first;
	}
	message->second.SetReceiver(receiver);
	return true;
}

bool MessageProcessor::AddSender(MESSAGE_TYPE type, int32_t groupIdx, int32_t processCount, int32_t bufferCount)
{
	auto message = m_messageList.find(type);
	if(message == m_messageList.end())
	{
		auto result = m_messageList.emplace(std::make_pair(type, message_processor_t()));
		if(false == result.second)
		{
			return false;
		}
		message = result.first;
	}	
	return message->second.AddSenderGroup(groupIdx, processCount, bufferCount);
}

bool MessageProcessor::SnedPacket_User(int32_t groupIdx, int32_t senderIdx, IocpSession* session, const char* packet, int32_t size)
{
	auto message = m_messageList.find(MESSAGE_TYPE::USER);
	if(message == m_messageList.end())
	{
		return false;
	}
	auto ringBuffer = message->second.GetRingBuffer(groupIdx, senderIdx);
	if(nullptr == ringBuffer)
	{
		return false;
	}
	auto pushData = ringBuffer->GetPushData();
	if(nullptr == pushData)
	{
		return false;
	}
	MessageUser* date = &pushData->m_user;
	date->SetData(session, packet, size);
	ringBuffer->PushCompleted();

	return true;
}

bool MessageProcessor::SnedPacket_User(int32_t groupIdx, int32_t senderIdx, protocol_svr::MESSAGE protocol, flatbuffers::FlatBufferBuilder& fbb)
{
	auto message = m_messageList.find(MESSAGE_TYPE::USER);
	if(message == m_messageList.end())
	{
		return false;
	}
	auto ringBuffer = message->second.GetRingBuffer(groupIdx, senderIdx);
	if(nullptr == ringBuffer)
	{
		return false;
	}
	auto pushData = ringBuffer->GetPushData();
	if(nullptr == pushData)
	{
		return false;
	}
	MessageBase* date = &pushData->m_base;	
	uint16_t packetSize = fbb.GetSize() + 4;
	memcpy(date->m_data,		&packetSize,			sizeof(uint16_t));
	memcpy(&date->m_data[2],	&protocol,				sizeof(protocol_svr::MESSAGE));
	memcpy(&date->m_data[4],	fbb.GetBufferPointer(),	fbb.GetSize());

	ringBuffer->PushCompleted();

	return true;
}

bool MessageProcessor::SnedPacket_DB(int32_t groupIdx, int32_t senderIdx, protocol_svr::MESSAGE protocol, flatbuffers::FlatBufferBuilder& fbb)
{
	auto message = m_messageList.find(MESSAGE_TYPE::DATABASE);
	if(message == m_messageList.end())
	{
		return false;
	}
	auto ringBuffer = message->second.GetRingBuffer(groupIdx, senderIdx);
	if(nullptr == ringBuffer)
	{
		return false;
	}
	auto pushData = ringBuffer->GetPushData();
	if(nullptr == pushData)
	{
		return false;
	}
	MessageBase* date = &pushData->m_base;	
	uint16_t packetSize = fbb.GetSize() + 4;
	memcpy(date->m_data,		&packetSize,			sizeof(uint16_t));
	memcpy(&date->m_data[2],	&protocol,				sizeof(protocol_svr::MESSAGE));
	memcpy(&date->m_data[4],	fbb.GetBufferPointer(),	fbb.GetSize());

	ringBuffer->PushCompleted();

	return true;
}


