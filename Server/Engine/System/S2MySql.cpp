#include "S2Engine.h"
#include "S2MySql.h"

#ifdef USE_MYSQL

#pragma comment(lib, "libmysql.lib")

namespace s2 {

S2MySql::S2MySql()
{
}

S2MySql::~S2MySql()
{
	Destroy();
}

bool S2MySql::Create(const char* host, const uint16_t port, const char* user, const char* pass, const char* database)
{
	m_connectInfo.SetInfo(host, port, user, pass, database);

	m_mysql = new MYSQL();
	if(nullptr == m_mysql)
	{
		return false;
	}
	mysql_init(m_mysql);

	Connection();

	return true;
}

void S2MySql::Destroy()
{
	if(nullptr != m_result)
	{
		mysql_free_result(m_result);
		m_result = nullptr;
	}
	if(nullptr != m_connection)
	{
		mysql_close(m_connection);
		m_connection = nullptr;
	}
	if(nullptr != m_mysql)
	{
		mysql_close(m_mysql);
		delete m_mysql;
		m_mysql = nullptr;
	}
}

bool S2MySql::Connection()
{
	if(nullptr != m_result)
	{
		mysql_free_result(m_result);
		m_result = nullptr;
	}
	if(nullptr != m_connection)
	{
		mysql_close(m_connection);
		m_connection = nullptr;
	}

	m_connection = mysql_real_connect(m_mysql, m_connectInfo.m_host.c_str(), m_connectInfo.m_user.c_str(), m_connectInfo.m_pass.c_str(), m_connectInfo.m_database.c_str(), m_connectInfo.m_port, nullptr, 0);
	if(nullptr == m_connection)
	{
		return false;
	}
	return true;
}

bool S2MySql::Execute(const char* query)
{
    int32_t queryStat = mysql_query(m_connection, query);
	if(0 != queryStat)
	{
		Connection();
		return false;
	}

    return true;
}

bool S2MySql::ExecuteSelect(const char* query)
{
    int queryStat = mysql_query(m_connection, query);
	if(0 != queryStat)
	{
		Connection();
		return false;
	}
	if(nullptr != m_result)
	{
		mysql_free_result(m_result);
		m_result = nullptr;
	}
	m_result = mysql_store_result(m_connection);

	MoveNext();

	return true;
}

bool S2MySql::MoveNext()
{
	m_row = mysql_fetch_row(m_result);
	if(nullptr == m_row)
		return false;
		
	return true;
}

bool S2MySql::TransBegin()
{
	return mysql_autocommit(m_connection, false);
}

bool S2MySql::TransCommit()
{
	return mysql_commit(m_connection);
}

bool S2MySql::TransRollback()
{
	return mysql_rollback(m_connection);
}

uint64_t S2MySql::GetRowCount()
{
	return m_result->row_count;
}

uint32_t S2MySql::GetColumnCount()
{
	return m_result->field_count;
}

char* S2MySql::GetColumn_char(int32_t idx)
{
	return m_row[idx];
}
int8_t S2MySql::GetColumn_int8(int32_t idx)
{
	return static_cast<int8_t>(atoi(m_row[idx]));
}
uint8_t S2MySql::GetColumn_uint8(int32_t idx)
{
	return static_cast<uint8_t>(atoi(m_row[idx]));
}
int16_t S2MySql::GetColumn_int16(int32_t idx)
{
	return static_cast<int16_t>(atoi(m_row[idx]));
}
uint16_t S2MySql::GetColumn_uint16(int32_t idx)
{
	return static_cast<uint16_t>(atoi(m_row[idx]));
}
int32_t	S2MySql::GetColumn_int32(int32_t idx)
{
	return static_cast<int32_t>(atoi(m_row[idx]));
}
uint32_t S2MySql::GetColumn_uint32(int32_t idx)
{
	return static_cast<uint32_t>(atoi(m_row[idx]));
}
int64_t S2MySql::GetColumn_int64(int32_t idx)
{
	return static_cast<int64_t>(atoll(m_row[idx]));
}
uint64_t S2MySql::GetColumn_uint64(int32_t idx)
{
	return static_cast<uint64_t>(atoll(m_row[idx]));
}
}

#endif