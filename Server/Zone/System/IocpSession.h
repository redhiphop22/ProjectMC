#pragma once

class IocpSession : public s2::S2IocpSession
{
public:
	IocpSession(int32_t idx);
	~IocpSession();

	void					CompletionLogin(uid_t UID);
	uid_t					GetUID()			{	return m_UID;	}

	bool					SendPacket(protocol::MESSAGE messageID, flatbuffers::FlatBufferBuilder& fbb);

	virtual bool			OnAccept() override;
	virtual bool			OnClose() override;
	virtual bool			OnRecvPacket(int32_t idx, const char* packet, int32_t size) override;	
protected:

	uid_t					m_UID = 0;
};