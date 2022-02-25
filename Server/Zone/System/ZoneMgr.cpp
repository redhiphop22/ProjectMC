#include "Zone.h"
#include "ZoneMgr.h"
#include "ZoneInfo.h"

ZoneMgr::ZoneMgr()
{
}

ZoneMgr::~ZoneMgr()
{
}

bool ZoneMgr::Create()
{
	return true;
}

void ZoneMgr::Destroy()
{
}

bool ZoneMgr::CreateZone(int32_t mapId)
{
	ZoneInfo* zoneInfo = new ZoneInfo(mapId);
	if(nullptr == zoneInfo)
	{
		return false;
	}
	m_zoneList.emplace(mapId, zoneInfo);

	return true;
}

bool ZoneMgr::EnterZone(IocpSession* session, const protocol_svr::ENTER_MAP_REQ* msg)
{
	auto zoneInfo = m_zoneList.find(msg->map_id());
	if(zoneInfo == m_zoneList.end())
	{
		return false;
	}
	zoneInfo->second->EnterZone(session, msg->entity_info());
	return true;
}

bool ZoneMgr::LeaveZone(int32_t mapId, uid_t uid)
{
	auto zoneInfo = m_zoneList.find(mapId);
	if(zoneInfo == m_zoneList.end())
	{
		return false;
	}
	zoneInfo->second->LeaveZone(uid);

	return true;
}

bool ZoneMgr::ENTITY_MOVE_VELOCITY_REQ(const protocol_svr::ENTITY_MOVE_VELOCITY_REQ* msg)
{
	auto zoneInfo = m_zoneList.find(msg->map_id());
	if(zoneInfo == m_zoneList.end())
	{
		return false;
	}
	zoneInfo->second->ENTITY_MOVE_VELOCITY_REQ(msg);

	return true;
}

bool ZoneMgr::ENTITY_MOVE_STOP_REQ(const protocol_svr::ENTITY_MOVE_STOP_REQ* msg)
{
	auto zoneInfo = m_zoneList.find(msg->map_id());
	if(zoneInfo == m_zoneList.end())
	{
		return false;
	}
	zoneInfo->second->ENTITY_MOVE_STOP_REQ(msg);

	return true;
}