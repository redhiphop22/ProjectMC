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

private:
	zone_list_t				m_zoneList;
};