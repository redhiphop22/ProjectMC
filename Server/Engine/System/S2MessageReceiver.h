#pragma once

namespace s2 {

class S2MessageReceiver
{
public:
	S2MessageReceiver() = default;
	virtual ~S2MessageReceiver() = default;

	virtual bool			OnMessageUpdate(int32_t groupIdx, void* data) = NULL;
};

}