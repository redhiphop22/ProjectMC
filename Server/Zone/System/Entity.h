#pragma once

class ZoneInfo;

class Entity
{
public:
	Entity(IocpSession* session, ZoneInfo* zoneInfo);
	~Entity();

	bool					Create(const common::ENTITY_INFO* entityInfo);

	uid_t					GetUID()	{	return m_uid; }

	flatbuffers::Offset<common::ENTITY_SPAWN_INFO>	GetSpawnInfo(flatbuffers::FlatBufferBuilder& fbb);

private:
	IocpSession*			m_session = nullptr;
	ZoneInfo*				m_zoneInfo = nullptr;

	uid_t					m_uid;
	std::string				m_nickName;

	CharacterFace			m_faceInfo;
	CharacterEquipmentList	m_equipmentList;
	vector3_t				m_position;
};