#pragma once

namespace s2 {

template<typename T>
class S2MessageProcessor
{	
	using sender_list_t = std::unordered_map<int32_t, S2RingBuffer<T>>;
	using group_list_t = std::unordered_map<int32_t, sender_list_t>;

public:
	S2MessageProcessor() = default;
	virtual ~S2MessageProcessor() = default;

	void Destroy()
	{
	}

	void MessageUpdate()
	{
		for(auto& group : m_groupList)
		{
			for(auto& sender : group.second)
			{
				auto popData = sender.second.GetPopData();
				if(nullptr == popData)
					continue;

				if(false == m_receiver->OnMessageUpdate(group.first, popData))
					continue;

				sender.second.PopCompleted();
			}
		}
	}
	
	void SetReceiver(S2MessageReceiver* receiver)
	{
		m_receiver = receiver;
	}

	bool AddSender(int32_t groupIdx, int32_t processIdx, int32_t bufferCount)
	{
		auto groupIter = m_groupList.find(groupIdx);
		if(groupIter == m_groupList.end())
		{
			auto result = m_groupList.emplace(groupIdx, sender_list_t());
			if(false == result.second)
			{
				return false;
			}
			groupIter = result.first;
		}

		auto& sender = groupIter->second;

		auto senderIter = sender.find(processIdx);
		if(senderIter != sender.end())
		{
			return false;
		}

		auto result = sender.emplace(processIdx, S2RingBuffer<T>());
		if(false == result.second)
		{
			return false;
		}
		if(false == result.first->second.Create(bufferCount))
		{
			return false;
		}

		return true;
	}

	bool PushMessage(int32_t groupIdx, int32_t senderIdx, const char* data, int32_t size)
	{
		auto groupIter = m_groupList.find(groupIdx);
		if(groupIter == m_groupList.end())
		{
			return false;
		}

		auto& sender = groupIter->second;

		auto senderIter = sender.find(senderIdx);
		if(senderIter == sender.end())
		{
			return false;
		}

		senderIter->second.PushData(data, size);
		return true;
	}

	S2RingBuffer<T>* GetRingBuffer(int32_t groupIdx, int32_t senderIdx)
	{
		auto groupIter = m_groupList.find(groupIdx);
		if(groupIter == m_groupList.end())
		{
			return nullptr;
		}

		auto& sender = groupIter->second;

		auto senderIter = sender.find(senderIdx);
		if(senderIter == sender.end())
		{
			return nullptr;
		}

		return &senderIter->second;
	}
protected:
	group_list_t			m_groupList;
	S2MessageReceiver*		m_receiver;
};

}