#pragma once

namespace s2 {

template<typename T>
class S2Singleton
{
public:
	S2Singleton() = default;
	virtual ~S2Singleton() = default;

	inline static T& GetInstance()
	{
		static T instance;
		return instance;
	}
};

}