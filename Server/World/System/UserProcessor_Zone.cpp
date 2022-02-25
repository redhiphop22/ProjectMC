#include "World.h"
#include "UserProcessor.h"
#include "UserInfo.h"
#include "UserMgr.h"

bool UserProcessor::RegisterPacket_Zone()
{
	REGIST_PACKET(m_zoneFunc.emplace(std::make_pair(protocol_svr::MESSAGE_ENTER_MAP_ACK, &UserProcessor::ENTER_MAP_ACK)));
	REGIST_PACKET(m_zoneFunc.emplace(std::make_pair(protocol_svr::MESSAGE_ENTITY_SPAWN_ACK, &UserProcessor::ENTITY_SPAWN_ACK)));
	REGIST_PACKET(m_zoneFunc.emplace(std::make_pair(protocol_svr::MESSAGE_ENTITY_DESTROY_ACK, &UserProcessor::ENTITY_DESTROY_ACK)));
	REGIST_PACKET(m_zoneFunc.emplace(std::make_pair(protocol_svr::MESSAGE_ENTITY_MOVE_VELOCITY_ACK, &UserProcessor::ENTITY_MOVE_VELOCITY_ACK)));
	REGIST_PACKET(m_zoneFunc.emplace(std::make_pair(protocol_svr::MESSAGE_ENTITY_MOVE_STOP_ACK, &UserProcessor::ENTITY_MOVE_STOP_ACK)));
	

	return true;
}

bool UserProcessor::ENTER_MAP_ACK(const char* buffer, int32_t size)
{
	PACKET_CONVERT_S2S(ENTER_MAP_ACK, buffer, size);
	
	uid_t uid = msg->uid();
	auto user = m_userMgr->GetUserInfo(uid);
	if(nullptr == user)
		return false;

	return user->ENTER_MAP_ACK(msg);
}

bool UserProcessor::ENTITY_SPAWN_ACK(const char* buffer, int32_t size)
{
	PACKET_CONVERT_S2S(ENTITY_SPAWN_ACK, buffer, size);
	
	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
	std::vector<flatbuffers::Offset<common::ENTITY_SPAWN_INFO>> spawnInfoList;
	for(auto iterSpawn = msg->spawn_list()->begin() ; iterSpawn != msg->spawn_list()->end() ; ++iterSpawn)
	{
		auto face = common::CreateCHARACTER_FACE(fbb,
			iterSpawn->face()->hair_idx(),
			iterSpawn->face()->hair_color(),
			iterSpawn->face()->face_idx(),
			iterSpawn->face()->face_color(),
			iterSpawn->face()->eye_idx(),
			iterSpawn->face()->eye_color(),
			iterSpawn->face()->eyebrow_idx(),
			iterSpawn->face()->eyebrow_color(),
			iterSpawn->face()->nose_idx(),
			iterSpawn->face()->nose_color(),
			iterSpawn->face()->mouth_idx(),
			iterSpawn->face()->mouth_color()
		);

		std::vector<flatbuffers::Offset<common::CHARACTER_EQUIPMENT>> equipmentList;
		for(auto iterEquipment = iterSpawn->equipment()->begin() ; iterEquipment != iterSpawn->equipment()->end() ; ++iterEquipment)
		{
			equipmentList.push_back(
				common::CreateCHARACTER_EQUIPMENT(fbb,
					iterEquipment->type(),
					iterEquipment->id()
				)
			);
		}
		
		spawnInfoList.push_back(common::CreateENTITY_SPAWN_INFO(fbb,
			iterSpawn->uid(),
			fbb.CreateString(iterSpawn->nick_name()->data()),
			face,
			fbb.CreateVector(equipmentList),
			iterSpawn->position())
		);
	}
	
	auto body = protocol::CreateENTITY_SPAWN_S2C(fbb,
		fbb.CreateVector(spawnInfoList)
	);
	fbb.Finish(body);

	for(auto iterUid = msg->uid_list()->begin() ; iterUid != msg->uid_list()->end() ; ++iterUid)
	{
		auto user = m_userMgr->GetUserInfo(*iterUid);
		if(nullptr == user)
			continue;

		user->SendPacket(protocol::MESSAGE_ENTITY_SPAWN_S2C, fbb);
	}

	return true;
}

bool UserProcessor::ENTITY_DESTROY_ACK(const char* buffer, int32_t size)
{
	PACKET_CONVERT_S2S(ENTITY_DESTROY_ACK, buffer, size);
	
	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
	auto body = protocol::CreateENTITY_DESTROY_S2C(fbb,
		msg->uid()
	);
	fbb.Finish(body);

	for(auto iterUid = msg->uid_list()->begin() ; iterUid != msg->uid_list()->end() ; ++iterUid)
	{
		auto user = m_userMgr->GetUserInfo(*iterUid);
		if(nullptr == user)
			continue;

		user->SendPacket(protocol::MESSAGE_ENTITY_DESTROY_S2C, fbb);
	}

	return true;
}

bool UserProcessor::ENTITY_MOVE_VELOCITY_ACK(const char* buffer, int32_t size)
{
	PACKET_CONVERT_S2S(ENTITY_MOVE_VELOCITY_ACK, buffer, size);
	
	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
	auto body = protocol::CreateENTITY_MOVE_VELOCITY_S2C(fbb,
		msg->result(),
		msg->uid(),
		msg->excute_time(),
		msg->position(),
		msg->forward(),
		msg->speed(),
		s2::S2Time::Now()
	);
	fbb.Finish(body);

	for(auto iterUid = msg->uid_list()->begin() ; iterUid != msg->uid_list()->end() ; ++iterUid)
	{
		auto user = m_userMgr->GetUserInfo(*iterUid);
		if(nullptr == user)
			continue;

		user->SendPacket(protocol::MESSAGE_ENTITY_MOVE_VELOCITY_S2C, fbb);
	}

	return true;
}

bool UserProcessor::ENTITY_MOVE_STOP_ACK(const char* buffer, int32_t size)
{
	PACKET_CONVERT_S2S(ENTITY_MOVE_STOP_ACK, buffer, size);
	
	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
	auto body = protocol::CreateENTITY_MOVE_STOP_S2C(fbb,
		msg->result(),
		msg->uid(),
		msg->excute_time(),
		msg->position(),
		s2::S2Time::Now()
	);
	fbb.Finish(body);

	for(auto iterUid = msg->uid_list()->begin() ; iterUid != msg->uid_list()->end() ; ++iterUid)
	{
		auto user = m_userMgr->GetUserInfo(*iterUid);
		if(nullptr == user)
			continue;

		user->SendPacket(protocol::MESSAGE_ENTITY_MOVE_STOP_S2C, fbb);
	}

	return true;
}