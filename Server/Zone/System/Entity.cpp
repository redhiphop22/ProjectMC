#include "Zone.h"
#include "Entity.h"
#include "ZoneInfo.h"

Entity::Entity(IocpSession* session, ZoneInfo* zoneInfo)
	: m_session(session)
	, m_zoneInfo(zoneInfo)
{
}

Entity::~Entity()
{
}

bool Entity::Create(const common::ENTITY_INFO* entityInfo)
{
	m_uid = entityInfo->uid();
	m_nickName = entityInfo->nick_name()->data();
	m_faceInfo.UnPacking(entityInfo->face());
	m_equipmentList.UnPacking(entityInfo->equipment());
	m_position.Reset();
	return true;
}

flatbuffers::Offset<common::ENTITY_SPAWN_INFO> Entity::GetSpawnInfo(flatbuffers::FlatBufferBuilder& fbb)
{
	common::VECTOR3 vector(m_position.X(), m_position.Y(), m_position.Z());

	return common::CreateENTITY_SPAWN_INFO(fbb,
		m_uid,
		fbb.CreateString(m_nickName.c_str()),
		m_faceInfo.Packing(fbb),
		fbb.CreateVector(m_equipmentList.Packing(fbb)),
		&vector
	);
}