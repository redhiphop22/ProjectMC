#pragma once

class DataBaseMgr : public s2::S2Thread, s2::S2ThreadPool, s2::S2MessageReceiver
{
public:
	DataBaseMgr();
	~DataBaseMgr();

	bool					Create(int32_t threadCount, int32_t bufferCount);
	void					Destroy();

	bool					SendPacket(protocol_svr::MESSAGE message, flatbuffers::FlatBufferBuilder& fbb);

	virtual bool			OnThreadUpdate() override;
	virtual bool			OnMessageUpdate(int32_t groupIdx, void* data) override;
protected:
};