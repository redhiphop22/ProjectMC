#pragma once

class ZoneInfo;

class ZoneMgr
{
	using zone_list_t = std::unordered_map<int32_t, ZoneInfo*>;
public:
	ZoneMgr();
	~ZoneMgr();

	bool					Create();
	void					Destroy();

	bool					CreateZone(int32_t mapId);

	bool					EnterZone(IocpSession* session, const protocol_svr::ENTER_MAP_REQ* msg);
	bool					LeaveZone(int32_t mapId, uid_t uid);

	bool					ENTITY_MOVE_VELOCITY_REQ(const protocol_svr::ENTITY_MOVE_VELOCITY_REQ* msg);
	bool					ENTITY_MOVE_STOP_REQ(const protocol_svr::ENTITY_MOVE_STOP_REQ* msg);

private:
	zone_list_t				m_zoneList;
};