#pragma once

#ifdef USE_MYSQL

#include <mysql.h>

namespace s2 {

class S2MySql
{
public:
	S2MySql();
	~S2MySql();

	bool					Create(const char* host, const uint16_t port, const char* user, const char* pass, const char* database);
	void					Destroy();
	
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
	MYSQL*					m_mysql = nullptr;
	MYSQL*					m_connection = nullptr;
	MYSQL_RES*				m_result = nullptr;
	MYSQL_ROW				m_row;
};

}

#endif