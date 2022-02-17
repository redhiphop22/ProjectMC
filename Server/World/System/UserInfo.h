#pragma once

class IocpSession;

class UserInfo
{
public:
	UserInfo(IocpSession* session, uid_t uid, common::ACCOUNT_AUTHORITY authority);
	~UserInfo();

	void					Destroy();

	IocpSession*			GetSession()				{	return m_session;	}
	bool					SendPacket(protocol::MESSAGE messageID, flatbuffers::FlatBufferBuilder& fbb);

public:
	flatbuffers::Offset<common::ENTITY_INFO> MakePacket_Entity(flatbuffers::FlatBufferBuilder& fbb);

public:
	bool					CHARACTER_INFO_DB_ACK(const protocol_svr::CHARACTER_INFO_DB_ACK* msg);
	bool					CHARACTER_CREATE_DB_ACK(const protocol_svr::CHARACTER_CREATE_DB_ACK* msg);
	bool					ENTER_MAP_ACK(const protocol_svr::ENTER_MAP_ACK* msg);

protected:

private:
	IocpSession*			m_session = nullptr;

	uid_t					m_uid;
	std::string				m_nickName;
	common::CHARACTER_TYPE	m_characterType;
	uint32_t				m_belong;

	CharacterFace			m_faceInfo;
	CharacterEquipmentList	m_equipmentList;

	map_t					m_mapId;
};