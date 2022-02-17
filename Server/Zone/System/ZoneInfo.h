#pragma once

class Entity;

class ZoneInfo
{
	using entity_list_t = std::unordered_map<uid_t, Entity*>;

public:
	ZoneInfo(int32_t mapId);
	~ZoneInfo();

	bool					Create();
	void					Destroy();

	void					Update();

	bool					EnterZone(IocpSession* session, const common::ENTITY_INFO* entityInfo);
	bool					LeaveZone(uid_t uid);

	void					OnUpdate();

	void					SpawnEntity(uid_t uid);
	void					DestroyEntity(uid_t uid);

private:
	map_t					m_mapId;
	entity_list_t			m_entityList;
	IocpSession*			m_session;

};