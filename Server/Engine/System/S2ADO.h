#pragma once

#ifdef USE_MSSQL

#import "c:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF", "EndOfFile")

namespace s2 {

namespace S2ADO {
	bool					Create();
	void					Destroy();
};

class S2ADOConnector
{

public:
	S2ADOConnector();
	~S2ADOConnector();

	bool					Connect(const char* connectFile);
	bool					Connect(const char* dataSource, const char* catalog, const char* userID, const char* userPW);
	bool					Close();

	bool					IsConnected();

	bool					ExecuteSP(const char* query);

	bool					Move(int32_t moveIdx);
	bool					MoveNext();
	bool					MoveFirst();
	bool					IsEOF();
	void					GetComError(_com_error &err, _ConnectionPtr connection);

	int32_t					GetRecordCount();
		
private:
	_ConnectionPtr			m_pConnection;
	_RecordsetPtr			m_pRecordSet;
	_CommandPtr				m_pCommand;
};

}

#endif