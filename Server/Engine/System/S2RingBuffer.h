#pragma once

namespace s2 {

template<typename T>
class S2RingBuffer
{
public:
	S2RingBuffer()
	{
	}
	~S2RingBuffer()
	{
		Destroy();
	}

	bool Create(int32_t maxCount)
	{
		m_maxCount = maxCount;
		m_data = new T[m_maxCount];
		if(nullptr == m_data)
		{
			//S2LOG2("Create Fail");
			//S2LOG("Create Fail");

			return false;
		}
		return true;
	}

	void Destroy()
	{
		SAFE_DELETE_ARRAY(m_data);
	}

	bool IsEmpty()
	{
		return m_frontPos == m_rearPos;
	}

	bool IsFull()
	{
		int32_t rearPos = m_rearPos + 1;
		rearPos = (rearPos < m_maxCount) ? rearPos : 0;

		return m_frontPos == rearPos;
	}

	int32_t Size()
	{
		return m_rearPos - m_frontPos + 1;
	}

	T* GetPushData()
	{
		if(IsFull())
		{
			return nullptr;
		}
		//while(IsFull())
		//{
		//	std::this_thread::sleep_for(std::chrono::milliseconds(1));
		//	//S2LOG("PushData Full");
		//}
		int32_t rearPos = m_rearPos + 1;
		rearPos = (rearPos < m_maxCount) ? rearPos : 0;
		return &m_data[rearPos];
	}
	void PushCompleted()
	{
		int32_t rearPos = m_rearPos + 1;
		m_rearPos = (rearPos < m_maxCount) ? rearPos : 0;
	}
	T* GetPopData()
	{
		if(IsEmpty())
		{
			return nullptr;
		}

		int32_t frontPos = m_frontPos + 1;
		frontPos = (frontPos < m_maxCount) ? frontPos : 0;
		return &m_data[frontPos];
	}
	void PopCompleted()
	{
		int32_t frontPos = m_frontPos + 1;
		m_frontPos = (frontPos < m_maxCount) ? frontPos : 0;
	}
	T* GetNextPopData()
	{
		int32_t frontPos = m_frontPos + 1;
		m_frontPos = (frontPos < m_maxCount) ? frontPos : 0;

		if(IsEmpty())
		{
			return nullptr;
		}

		frontPos = m_frontPos + 1;
		frontPos = (frontPos < m_maxCount) ? frontPos : 0;
		return &m_data[frontPos];
	}

	bool PushData(const T* data, int32_t size)
	{
		while(IsFull())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			//S2LOG("PushData Full");
		}

		int32_t rearPos = m_rearPos + 1;
		rearPos = (rearPos < m_maxCount) ? rearPos : 0;
		memcpy(m_data[rearPos], data, size);

		m_rearPos = rearPos;

		return true;
	}

	bool PopData(T* t)
	{
		if(IsEmpty())
		{
			return false;
		}

		int32_t frontPos = m_frontPos + 1;
		frontPos = (frontPos < m_maxCount) ? frontPos : 0;
		t = &m_data[frontPos];

		m_frontPos = frontPos;

		return true;
	}
private:
	T*					m_data = nullptr;
	int32_t				m_maxCount = 0;
	int32_t				m_rearPos = 0; 
	int32_t				m_frontPos = 0;
};

}