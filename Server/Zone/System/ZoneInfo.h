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

	void					OnUpdate();

	bool					EnterZone(IocpSession* session, const common::ENTITY_INFO* entityInfo);
	bool					LeaveZone(uid_t uid);

	void					SpawnEntity(uid_t uid);
	void					DestroyEntity(uid_t uid);

	bool					ENTITY_MOVE_VELOCITY_REQ(const protocol_svr::ENTITY_MOVE_VELOCITY_REQ* msg);
	bool					ENTITY_MOVE_STOP_REQ(const protocol_svr::ENTITY_MOVE_STOP_REQ* msg);

private:
	map_t					m_mapId;
	entity_list_t			m_entityList;
	IocpSession*			m_session;

};