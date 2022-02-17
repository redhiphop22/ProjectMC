#pragma once

class ProcessorBase
{
public:
	using processor_buffer_t = s2::S2RingBuffer<char[RING_BUFFER_ELEMENT_SIZE]>;

public:
	ProcessorBase();
	~ProcessorBase();

public:
	virtual processor_buffer_t*	GetRingBuffer(int32_t idx) = 0;
};