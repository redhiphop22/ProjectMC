#include "Zone.h"
#include "UserProcessor.h"
#include "ZoneMgr.h"

bool UserProcessor::LOGOUT_REQ(IocpSession* session, const char* buffer, int32_t size)
{
	PACKET_CONVERT_S2S(LOGOUT_REQ, buffer, size);

	m_zoneMgr->LeaveZone(msg->map_id(), msg->uid());

	return true;
}

bool UserProcessor::ENTER_MAP_REQ(IocpSession* session, const char* buffer, int32_t size)
{
	PACKET_CONVERT_S2S(ENTER_MAP_REQ, buffer, size);

	if(nullptr == msg->entity_info())
		return false;

	m_zoneMgr->EnterZone(session, msg);

	return true;
}

bool UserProcessor::ENTITY_MOVE_VELOCITY_REQ(IocpSession* session, const char* buffer, int32_t size)
{
	PACKET_CONVERT_S2S(ENTITY_MOVE_VELOCITY_REQ, buffer, size);

	m_zoneMgr->ENTITY_MOVE_VELOCITY_REQ(msg);

	return true;
}

bool UserProcessor::ENTITY_MOVE_STOP_REQ(IocpSession* session, const char* buffer, int32_t size)
{
	PACKET_CONVERT_S2S(ENTITY_MOVE_STOP_REQ, buffer, size);

	m_zoneMgr->ENTITY_MOVE_STOP_REQ(msg);

	return true;
}