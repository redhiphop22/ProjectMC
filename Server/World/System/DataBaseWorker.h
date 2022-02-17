#pragma once

#define QUERY_SIZE	4096

class DataBaseWorker
{
	using db_func_t = bool(DataBaseWorker::*)(const char* buffer, int32_t size);
	using db_func_list_t = std::unordered_map<protocol_svr::MESSAGE, db_func_t>;
public:
	enum class WORKER_STATE
	{
		INIT,
		RUN,
		FINISH,
	};

public:
	DataBaseWorker(int32_t idx);
	~DataBaseWorker();

	bool					Create(const char* host, const uint16_t port, const char* user, const char* pass, const char* database);
	void					Destroy();
	void					OnUpdate();

	void					SetState(WORKER_STATE state)	{	m_state = state; }
	WORKER_STATE			GetState()						{	return m_state;	}
	bool					PushQuery(const char* query);

protected:
	bool					RegistPacket();
	bool					SendPacket(protocol_svr::MESSAGE message, flatbuffers::FlatBufferBuilder& fbb);

	bool					SERVER_CONNECT_DB_REQ(const char* buffer, int32_t size);
	bool					CHARACTER_INFO_DB_REQ(const char* buffer, int32_t size);
	bool					CHARACTER_NAME_DUPLICATION_DB_REQ(const char* buffer, int32_t size);
	bool					CHARACTER_CREATE_DB_REQ(const char* buffer, int32_t size);

private:
	s2::S2Thread			m_thread;

	int32_t					m_idx;
	s2::S2MySql				m_db;
	char					m_pushData[MESSAGE_BUFFER_SIZE];
	WORKER_STATE			m_state = WORKER_STATE::INIT;
	db_func_list_t			m_dbFunc;
};