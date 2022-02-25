#pragma once

#ifdef USE_MYSQL

#include <mysql.h>

namespace s2 {

class S2MySql
{
	struct ConnectInfo
	{
		std::string			m_host;
		std::string			m_user;
		std::string			m_pass;
		std::string			m_database;
		uint16_t			m_port;

		void SetInfo(const char* host, const uint16_t port, const char* user, const char* pass, const char* database)
		{
			m_host = host;
			m_user = user;
			m_pass = pass;
			m_database = database;
			m_port = port;
		}
	};

public:
	S2MySql();
	~S2MySql();

	bool					Create(const char* host, const uint16_t port, const char* user, const char* pass, const char* database);
	void					Destroy();
	
	time_t					GetLastRestTime()	{	return m_lastRestTime;	}

	bool					Connection();
	bool					Execute(const char* query);
	bool					ExecuteSelect(const char* query);
		
	bool					MoveNext();

	bool					TransBegin();
	bool					TransCommit();
	bool					TransRollback();

	uint64_t				GetRowCount();
	uint32_t				GetColumnCount();
		
	char*					GetColumn_char(int32_t idx);
	int8_t					GetColumn_int8(int32_t idx);
	uint8_t					GetColumn_uint8(int32_t idx);
	int16_t					GetColumn_int16(int32_t idx);
	uint16_t				GetColumn_uint16(int32_t idx);
	int32_t					GetColumn_int32(int32_t idx);
	uint32_t				GetColumn_uint32(int32_t idx);
	int64_t					GetColumn_int64(int32_t idx);
	uint64_t				GetColumn_uint64(int32_t idx);
	
private:
	ConnectInfo				m_connectInfo;

	MYSQL*					m_mysql = nullptr;
	MYSQL*					m_connection = nullptr;
	MYSQL_RES*				m_result = nullptr;
	MYSQL_ROW				m_row;

	time_t					m_lastRestTime;
};

}

#endif