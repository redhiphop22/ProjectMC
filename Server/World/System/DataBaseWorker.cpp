#include "World.h"
#include "DataBaseWorker.h"

#define ACCOUNT_AUTHORITY_INIT	common::ACCOUNT_AUTHORITY_GUEST

#define SQL_SUCCESS_SEND(p, r) {\
	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE); \
	auto body = protocol_svr::Create##p(fbb, uid, r); \
	fbb.Finish(body); \
	SendPacket(protocol_svr::MESSAGE_##p, fbb);\
	}

#define SQL_ERROR_SEND(p, r) {\
	S2LOG("Query : %s", query); \
	SQL_SUCCESS_SEND(p, r); \
	}

DataBaseWorker::DataBaseWorker(int32_t idx)
	: m_idx(idx)
{
}

DataBaseWorker::~DataBaseWorker()
{
}

bool DataBaseWorker::RegistPacket()
{
	REGIST_PACKET(m_dbFunc.emplace(std::make_pair(protocol_svr::MESSAGE_SERVER_CONNECT_DB_REQ, &DataBaseWorker::SERVER_CONNECT_DB_REQ)));
	REGIST_PACKET(m_dbFunc.emplace(std::make_pair(protocol_svr::MESSAGE_CHARACTER_INFO_DB_REQ, &DataBaseWorker::CHARACTER_INFO_DB_REQ)));
	REGIST_PACKET(m_dbFunc.emplace(std::make_pair(protocol_svr::MESSAGE_CHARACTER_NAME_DUPLICATION_DB_REQ, &DataBaseWorker::CHARACTER_NAME_DUPLICATION_DB_REQ)));
	REGIST_PACKET(m_dbFunc.emplace(std::make_pair(protocol_svr::MESSAGE_CHARACTER_CREATE_DB_REQ, &DataBaseWorker::CHARACTER_CREATE_DB_REQ)));

	return true;
}

bool DataBaseWorker::Create(const char* host, const uint16_t port, const char* user, const char* pass, const char* database)
{
	RegistPacket();

	if(false == m_db.Create(host, port, user, pass, database))
	{
		return false;
	}

	auto threadProc = [this]() -> void {
		this->OnUpdate();
	};
	return m_thread.ThreadCreateFunc(threadProc);
}

void DataBaseWorker::Destroy()
{
	m_thread.DestroyThread();
	m_db.Destroy();
}

bool DataBaseWorker::PushQuery(const char* query)
{	
	memcpy(m_pushData, query, MESSAGE_BUFFER_SIZE);
	m_state = WORKER_STATE::RUN;

	return true;
}

void DataBaseWorker::OnUpdate()
{
	THREAD_UPDATE_START;

	if(WORKER_STATE::RUN != m_state)
		return false;	

	MessageProcessor::MessageBase* data = (MessageProcessor::MessageBase*)m_pushData;

	MAKE_PACKET_HEADER_S2S(data->m_data);

	auto iter = m_dbFunc.find(static_cast<protocol_svr::MESSAGE>(packetProtocol));
	if(iter == m_dbFunc.end())
	{
		return false;
	}
	(this->*(iter->second))(body, packetSize);

	m_state = WORKER_STATE::FINISH;

	THREAD_UPDATE_END;
}

bool DataBaseWorker::SendPacket(protocol_svr::MESSAGE message, flatbuffers::FlatBufferBuilder& fbb)
{
	return MESSAGE_PROCESSOR().SnedPacket_User(static_cast<int32_t>(MessageProcessor::MESSAGE_GROUP_USER::DATABASE), m_idx, message, fbb);
}

bool DataBaseWorker::SERVER_CONNECT_DB_REQ(const char* buffer, int32_t size)
{
	PACKET_CONVERT_S2S(SERVER_CONNECT_DB_REQ, buffer, size);

	uid_t uid = msg->uid();
	char query[QUERY_SIZE];

	s2::string::Format(query, QUERY_SIZE, 
		"SELECT `authority`,`block_type`,UNIX_TIMESTAMP(`block_date`) FROM `account` WHERE (`uid`=%llu)", uid);

	common::ACCOUNT_AUTHORITY authority = ACCOUNT_AUTHORITY_INIT;
	common::ACCOUNT_BLOCK_TYPE block_type = common::ACCOUNT_BLOCK_TYPE_NONE;
	time_t block_date = 0;

	if(false == m_db.ExecuteSelect(query))
	{
		SQL_ERROR_SEND(SERVER_CONNECT_DB_ACK, common::RESULT_CODE_ERROR_DB_FAIL);
		return true;
	}
	if(0 == m_db.GetRowCount())
	{
		std::string id = msg->id()->data();
		s2::string::Format(query, QUERY_SIZE, 
			"INSERT INTO `account` (`uid`,`id`,`authority`) VALUES (%llu, '%s', %u)", uid, id.c_str(), ACCOUNT_AUTHORITY_INIT);
		if(false == m_db.Execute(query))
		{	
			SQL_ERROR_SEND(SERVER_CONNECT_DB_ACK, common::RESULT_CODE_ERROR_DB_FAIL);
			return true;
		}
	}
	else
	{
		authority = static_cast<common::ACCOUNT_AUTHORITY>(m_db.GetColumn_uint8(0));	// authority,
		block_type = static_cast<common::ACCOUNT_BLOCK_TYPE>(m_db.GetColumn_uint8(1));	// block_type
		block_date = m_db.GetColumn_int64(2);	// block_date
	}
	
	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
	auto bodyFbb = protocol_svr::CreateSERVER_CONNECT_DB_ACK(fbb, 
		uid,
		common::RESULT_CODE_SUCCESS,
		authority,
		block_type,
		block_date
	);
	fbb.Finish(bodyFbb);
	SendPacket(protocol_svr::MESSAGE_SERVER_CONNECT_DB_ACK, fbb);

	return true;
}

bool DataBaseWorker::CHARACTER_INFO_DB_REQ(const char* buffer, int32_t size)
{
	PACKET_CONVERT_S2S(CHARACTER_INFO_DB_REQ, buffer, size);

	uid_t uid = msg->uid();
	char query[QUERY_SIZE];

	s2::string::Format(query, QUERY_SIZE, 
		"SELECT `name`,'type',`belong`,`hair_idx`,`hair_color`,`face_idx`,`face_color`,`eye_idx`,`eye_color`,"
		"`eyebrow_idx`,`eyebrow_color`,`nose_idx`,`nose_color`,`mouth_idx`,`mouth_color` "
		"FROM `character` WHERE (`uid`=%llu)", uid);

	common::RESULT_CODE result = common::RESULT_CODE_SUCCESS;
	if(false == m_db.ExecuteSelect(query))
	{
		SQL_ERROR_SEND(CHARACTER_INFO_DB_ACK, common::RESULT_CODE_ERROR_DB_FAIL);
		return true;
	}
	if(0 == m_db.GetRowCount())
	{
		SQL_SUCCESS_SEND(CHARACTER_INFO_DB_ACK, common::RESULT_CODE_SUCCESS_CHARACTER_NONE);
		return true;
	}

	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);

	std::string name = m_db.GetColumn_char(0);
	common::CHARACTER_TYPE type = static_cast<common::CHARACTER_TYPE>(m_db.GetColumn_uint8(1));
	uint16_t belong = m_db.GetColumn_uint16(2);
	auto face = common::CreateCHARACTER_FACE(fbb,
		m_db.GetColumn_uint16(3),	// hair_idx
		m_db.GetColumn_uint8(4),	// hair_color
		m_db.GetColumn_uint16(5),	// face_idx
		m_db.GetColumn_uint8(6),	// face_color
		m_db.GetColumn_uint16(7),	// eye_idx
		m_db.GetColumn_uint8(8),	// eye_color
		m_db.GetColumn_uint16(9),	// eyebrow_idx
		m_db.GetColumn_uint8(10),	// eyebrow_color
		m_db.GetColumn_uint16(11),	// nose_idx
		m_db.GetColumn_uint8(12),	// nose_color
		m_db.GetColumn_uint16(13),	// mouth_idx
		m_db.GetColumn_uint8(14)	// mouth_color
	);

	s2::string::Format(query, QUERY_SIZE, 
		"SELECT `top`,`bottom`,`onepiece`,`shoes`,`eyelash`,`eyeshadow`,`facial_hair`,`piercing`,"
		"`tattoo`,`hat`,`glasses`,`earring`,`necklace`,`watch`,`bag`,`smartphone` "
		"FROM `character_equipment` WHERE (`uid`=%llu)", uid);

	if(false == m_db.ExecuteSelect(query) || 0 == m_db.GetRowCount())
	{
		SQL_ERROR_SEND(CHARACTER_INFO_DB_ACK, common::RESULT_CODE_SUCCESS_CHARACTER_NONE);
		return true;
	}
	std::vector<flatbuffers::Offset<common::CHARACTER_EQUIPMENT>> equipmentList;
	for(uint8_t i = 0 ; i < common::CHARACTER_EQUIPMENT_TYPE_MAX ; ++i)
	{
		itemID_t itemID = m_db.GetColumn_uint32(i);
		if(0 == itemID)			continue;
		equipmentList.emplace_back(
			common::CreateCHARACTER_EQUIPMENT(fbb,
				static_cast<common::CHARACTER_EQUIPMENT_TYPE>(i),
				itemID
			)
		);
	}

	common::VECTOR3 position(0.f, 0.f, 0.f);
	auto bodyFbb = protocol_svr::CreateCHARACTER_INFO_DB_ACK(fbb,
		uid,
		result,
		fbb.CreateString(name.c_str()),
		type,
		belong,
		face,
		fbb.CreateVector(equipmentList),
		1,
		&position
	);
	fbb.Finish(bodyFbb);
	SendPacket(protocol_svr::MESSAGE_CHARACTER_INFO_DB_ACK, fbb);
	
	return true;
}

bool DataBaseWorker::CHARACTER_NAME_DUPLICATION_DB_REQ(const char* buffer, int32_t size)
{
	PACKET_CONVERT_S2S(CHARACTER_NAME_DUPLICATION_DB_REQ, buffer, size);

	uid_t uid = msg->uid();
	char query[QUERY_SIZE];

	s2::string::Format(query, QUERY_SIZE, 
		"SELECT `uid` "
		"FROM `character` WHERE (`name`='%s')", msg->nick_name()->c_str());

	if(false == m_db.ExecuteSelect(query))
	{
		SQL_ERROR_SEND(CHARACTER_INFO_DB_ACK, common::RESULT_CODE_ERROR_DB_FAIL);
		return true;
	}

	common::RESULT_CODE result;
	if(0 != m_db.GetRowCount())
	{
		result = common::RESULT_CODE_ERROR_CHARACTER_NAME_DUPLICATION;
	}
	else
	{
		result = common::RESULT_CODE_SUCCESS;
	}

	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
	auto bodyFbb = protocol_svr::CreateCHARACTER_NAME_DUPLICATION_DB_ACK(fbb,
		uid,
		result,
		fbb.CreateString(msg->nick_name())
	);
	fbb.Finish(bodyFbb);
	SendPacket(protocol_svr::MESSAGE_CHARACTER_NAME_DUPLICATION_DB_ACK, fbb);

	return true;
}

bool DataBaseWorker::CHARACTER_CREATE_DB_REQ(const char* buffer, int32_t size)
{
	PACKET_CONVERT_S2S(CHARACTER_CREATE_DB_REQ, buffer, size);

	char query[QUERY_SIZE];

	uid_t uid = msg->uid();

	m_db.TransBegin();

	auto face = msg->face();
	s2::string::Format(query, QUERY_SIZE, 
		"INSERT INTO `character` (`uid`,`name`,`type`,`belong`,`hair_idx`,`hair_color`,`face_idx`,`face_color`,"
		"`eye_idx`,`eye_color`,`eyebrow_idx`,`eyebrow_color`,`nose_idx`,`nose_color`,`mouth_idx`,`mouth_color`) "
		"VALUES (%llu,'%s',%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u)", 
		uid, msg->nick_name()->data(), msg->type(), msg->belong(), face->hair_idx(), face->hair_color(), face->face_idx(), face->face_color(),
		face->eye_idx(), face->eye_color(), face->eyebrow_idx(), face->eyebrow_color(), face->nose_idx(), face->nose_color(), face->mouth_idx(), face->mouth_color());
			
	if(false == m_db.Execute(query))
	{
		m_db.TransRollback();
		SQL_ERROR_SEND(CHARACTER_CREATE_DB_ACK, common::RESULT_CODE_ERROR_DB_FAIL);
		return true;
	}

	itemID_t ItemID[common::CHARACTER_EQUIPMENT_TYPE_MAX+1] = {0,};
	auto equipmentList = msg->equipment();
	for(uint32_t i = 0 ; i < equipmentList->size() ; ++i)
	{
		ItemID[static_cast<int32_t>(equipmentList->Get(i)->type())] = equipmentList->Get(i)->id();
	}
	s2::string::Format(query, QUERY_SIZE, 
		"INSERT INTO `character_equipment` (`uid`,`top`,`bottom`,`onepiece`,`shoes`,`eyelash`,`eyeshadow`,"
		"`facial_hair`,`piercing`,`tattoo`,`hat`,`glasses`,`earring`,`necklace`,`watch`,`bag`,`smartphone`) "
		"VALUES (%llu,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u);", 
		uid, ItemID[0], ItemID[1], ItemID[2], ItemID[3], ItemID[4], ItemID[5], ItemID[6], ItemID[7], ItemID[8], ItemID[9], 
		ItemID[10], ItemID[11], ItemID[12], ItemID[13], ItemID[14], ItemID[15]);

	if(false == m_db.Execute(query))
	{	
		m_db.TransRollback();
		SQL_ERROR_SEND(CHARACTER_CREATE_DB_ACK, common::RESULT_CODE_ERROR_DB_FAIL);
		return true;
	}
	m_db.TransCommit();

	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
	auto faceFbb = common::CreateCHARACTER_FACE(fbb,
		face->hair_idx(),
		face->hair_color(),
		face->face_idx(),
		face->face_color(),
		face->eye_idx(),
		face->eye_color(),
		face->eyebrow_idx(),
		face->eyebrow_color(),
		face->nose_idx(),
		face->nose_color(),
		face->mouth_idx(),
		face->mouth_color()
	);
	std::vector<flatbuffers::Offset<common::CHARACTER_EQUIPMENT>> equipmentFbb;
	for(uint32_t i = 0 ; i < equipmentList->size() ; ++i)
	{
		equipmentFbb.emplace_back(
			common::CreateCHARACTER_EQUIPMENT(fbb,
				equipmentList->Get(i)->type(),
				equipmentList->Get(i)->id()
			)
		);
	}
	common::VECTOR3 position(0.f, 0.f, 0.f);
	auto bodyFbb = protocol_svr::CreateCHARACTER_CREATE_DB_ACK(fbb,
		uid,
		common::RESULT_CODE_SUCCESS,
		fbb.CreateString(msg->nick_name()->data()),
		msg->type(),
		msg->belong(),
		faceFbb,
		fbb.CreateVector(equipmentFbb),
		1,
		&position
	);
	fbb.Finish(bodyFbb);
	SendPacket(protocol_svr::MESSAGE_CHARACTER_CREATE_DB_ACK, fbb);

	return true;
}
