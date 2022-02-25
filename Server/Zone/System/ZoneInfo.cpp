#include "Zone.h"
#include "ZoneInfo.h"

#include "IocpSession.h"
#include "Entity.h"

ZoneInfo::ZoneInfo(int32_t mapId)
	: m_mapId(mapId)
{
}

ZoneInfo::~ZoneInfo()
{
}

bool ZoneInfo::Create()
{
	return true;
}

void ZoneInfo::Destroy()
{
}

void ZoneInfo::OnUpdate()
{
}

bool ZoneInfo::EnterZone(IocpSession* session, const common::ENTITY_INFO* entityInfo)
{
	// 임시처리
	m_session = session;

	uid_t uid = entityInfo->uid();
	auto entityItr = m_entityList.find(uid);
	if(entityItr == m_entityList.end())
	{
		auto entity = new Entity(session, this);
		if(nullptr == entity)
		{
			return false;
		}
		entity->Create(entityInfo);

		m_entityList.emplace(uid, entity);
	}

	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
	auto body = protocol_svr::CreateENTER_MAP_ACK(fbb, 
		uid,
		common::RESULT_CODE_SUCCESS,
		m_mapId);
	fbb.Finish(body);
	m_session->SendPacket(protocol_svr::MESSAGE_ENTER_MAP_ACK, fbb);
	
	SpawnEntity(uid);
	
	return true;
}

bool ZoneInfo::LeaveZone(uid_t uid)
{
	DestroyEntity(uid);
	m_entityList.erase(uid);

	return true;
}

void ZoneInfo::SpawnEntity(uid_t uid)
{
	flatbuffers::FlatBufferBuilder fbbMe(FBB_BASIC_SIZE);
	flatbuffers::FlatBufferBuilder fbbOther(FBB_BASIC_SIZE);

	std::vector<uid_t> uidFbbMe;
	std::vector<uid_t> uidFbbOther;
	std::vector<flatbuffers::Offset<common::ENTITY_SPAWN_INFO>> spwanInfoFbbMe;
	std::vector<flatbuffers::Offset<common::ENTITY_SPAWN_INFO>> spwanInfoFbbOther;
	for(auto& iter : m_entityList)
	{
		auto& entity = iter.second;
		if(uid == entity->GetUID())
		{
			uidFbbMe.push_back(entity->GetUID());
			spwanInfoFbbOther.push_back(entity->GetSpawnInfo(fbbOther));
		}
		else
		{
			uidFbbOther.push_back(entity->GetUID());			
			spwanInfoFbbMe.push_back(entity->GetSpawnInfo(fbbMe));
		}
	}

	if(0 < uidFbbOther.size())
	{
		{
			auto body = protocol_svr::CreateENTITY_SPAWN_ACK(fbbMe, 
				fbbMe.CreateVector(uidFbbMe),
				fbbMe.CreateVector(spwanInfoFbbMe)
			);
			fbbMe.Finish(body);
			m_session->SendPacket(protocol_svr::MESSAGE_ENTITY_SPAWN_ACK, fbbMe);
		}
		{
			auto body = protocol_svr::CreateENTITY_SPAWN_ACK(fbbOther, 
				fbbOther.CreateVector(uidFbbOther),
				fbbOther.CreateVector(spwanInfoFbbOther)
			);
			fbbOther.Finish(body);
			m_session->SendPacket(protocol_svr::MESSAGE_ENTITY_SPAWN_ACK, fbbOther);
		}
	}
}

void ZoneInfo::DestroyEntity(uid_t uid)
{
	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);

	std::vector<uint64_t> uidFbb;
	for(auto& iter : m_entityList)
	{
		auto& entity = iter.second;
		if(uid == entity->GetUID())
			continue;
		
		uidFbb.push_back(entity->GetUID());			
	}

	if(0 < uidFbb.size())
	{
		auto body = protocol_svr::CreateENTITY_DESTROY_ACK(fbb, 
			fbb.CreateVector(uidFbb),
			uid
		);
		fbb.Finish(body);
		m_session->SendPacket(protocol_svr::MESSAGE_ENTITY_DESTROY_ACK, fbb);
	}
}

bool ZoneInfo::ENTITY_MOVE_VELOCITY_REQ(const protocol_svr::ENTITY_MOVE_VELOCITY_REQ* msg)
{
	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);

	uid_t uid = msg->uid();

	auto entity = m_entityList.find(uid);
	if(entity == m_entityList.end())
	{
		return false;
	}

	std::vector<uint64_t> uidFbb;
	for(auto& iter : m_entityList)
	{
		auto& entity = iter.second;
		uidFbb.push_back(entity->GetUID());
	}

	auto body = protocol_svr::CreateENTITY_MOVE_VELOCITY_ACK(fbb, 
		fbb.CreateVector(uidFbb),
		common::RESULT_CODE_SUCCESS,
		msg->uid(),
		msg->excute_time(),
		msg->position(),
		msg->forward(),
		msg->speed()
	);
	fbb.Finish(body);

	m_session->SendPacket(protocol_svr::MESSAGE_ENTITY_MOVE_VELOCITY_ACK, fbb);

	return true;
}

bool ZoneInfo::ENTITY_MOVE_STOP_REQ(const protocol_svr::ENTITY_MOVE_STOP_REQ* msg)
{
	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);

	uid_t uid = msg->uid();

	auto entity = m_entityList.find(uid);
	if(entity == m_entityList.end())
	{
		return false;
	}

	std::vector<uint64_t> uidFbb;
	for(auto& iter : m_entityList)
	{
		auto& entity = iter.second;
		uidFbb.push_back(entity->GetUID());
	}

	auto body = protocol_svr::CreateENTITY_MOVE_STOP_ACK(fbb, 
		fbb.CreateVector(uidFbb),
		common::RESULT_CODE_SUCCESS,
		msg->uid(),
		msg->excute_time(),
		msg->position()
	);
	fbb.Finish(body);

	m_session->SendPacket(protocol_svr::MESSAGE_ENTITY_MOVE_STOP_ACK, fbb);

	return true;
}