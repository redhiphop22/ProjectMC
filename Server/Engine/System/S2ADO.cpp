#include "S2Engine.h"

#ifdef USE_MSSQL

namespace s2 {

namespace S2ADO {

bool Create()
{
	if (FAILED(::CoInitialize(NULL)))
		return false;

	return true;
}
void Destroy()
{
	::CoUninitialize();
}

}

S2ADOConnector::S2ADOConnector()
{
}

S2ADOConnector::~S2ADOConnector()
{
}

bool S2ADOConnector::Connect(const char* connectFile )
{
	//Initialize the COM Library
	if(FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)))
		return false;

	try
	{
		m_pConnection = _ConnectionPtr(__uuidof(Connection));
		//m_pCommand = _CommandPtr(__uuidof(Command));		
		m_pConnection->Open(_bstr_t(connectFile), _bstr_t(""), _bstr_t(""), adCmdUnspecified);
		m_pRecordSet = _RecordsetPtr(__uuidof(Recordset));
		m_pRecordSet->putref_ActiveConnection(m_pConnection);	
	}
	catch (_com_error & err)
	{
		GetComError(err, m_pConnection);
		return false;
	}

	return true;
}

bool S2ADOConnector::Connect(const char* dataSource, const char* catalog, const char* userID, const char* userPW)
{
	std::string connectInfo;
	s2::string::Format(connectInfo, "Provider='SQLOLEDB.1';Data Source='%s';Initial Catalog='%s';Persist Security Info='True'",
		dataSource, catalog );

	//Initialize the COM Library
	if(FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)))
		return false;

	try
	{
		m_pConnection = _ConnectionPtr(__uuidof(Connection));
		//m_pCommand = _CommandPtr(__uuidof(Command));		
		m_pConnection->Open(_bstr_t(connectInfo.c_str()), _bstr_t(userID), _bstr_t(userPW), adCmdUnspecified);
		m_pRecordSet = _RecordsetPtr(__uuidof(Recordset));
		m_pRecordSet->putref_ActiveConnection(m_pConnection);	
	}
	catch (_com_error & err)
	{
		GetComError(err, m_pConnection);
		return false;
	}

	return true;
}

bool S2ADOConnector::Close()
{
	try
	{
		if (m_pRecordSet == NULL) return TRUE;

		if (m_pRecordSet->GetState() == adStateOpen)
		{
			m_pRecordSet->Close();	
			//if (TRUE == m_bCursorLocation_adUserClientFlag)
			//{
			//	m_pRecordSet->Release();
			//}
		}

		if (m_pConnection == NULL) return TRUE;

		if (m_pConnection->State == adStateOpen)
		{
			m_pConnection->Close();
		}

		m_pConnection = NULL;
	}
	catch (_com_error& err)
	{
		GetComError(err, m_pConnection);	   	   
		return false;
	}

	CoUninitialize();

	return true;
}

bool S2ADOConnector::IsConnected()
{
	if(NULL == m_pConnection)
		return false;

	if(m_pConnection->GetState() != adStateOpen)
		return false;

	return true;
}

bool S2ADOConnector::ExecuteSP(const char* query)
{
	if(NULL == m_pConnection)
		return false;

	_variant_t vtConnection((IDispatch*)m_pConnection);
	_variant_t vtQry(query);

	try
	{		
		if (m_pRecordSet->GetState() == adStateOpen)
		{
			m_pRecordSet->Close();			
			m_pRecordSet->Release();
			//m_pRecordSet = _RecordsetPtr(__uuidof(Recordset));
			//m_pRecordSet->putref_ActiveConnection(m_pConnection);	
		}		

		m_pRecordSet->CursorLocation = adUseClient; // Don't use 'adUseServer' option.
		m_pRecordSet->Open(vtQry, vtConnection, adOpenStatic, adLockUnspecified, adCmdUnspecified );	
		m_pRecordSet->AddRef(); // release it externally.		
	}
	catch (_com_error& err)
	{
		GetComError(err, m_pConnection);	   	   
		return false;
	}

	return true;
}

bool S2ADOConnector::Move(int32_t moveIdx)
{
	try
	{
		m_pRecordSet->Move(moveIdx);
	}
	catch (_com_error& err)
	{
		GetComError(err, m_pConnection);	   	   
		return false;
	}

	return true;
}

bool S2ADOConnector::MoveNext()
{
	try
	{
		m_pRecordSet->MoveNext();
	}
	catch (_com_error& err)
	{
		GetComError(err, m_pConnection);	   	   
		return false;
	}

	return true;
}

bool S2ADOConnector::MoveFirst()
{
	try
	{
		m_pRecordSet->MoveFirst();
	}
	catch (_com_error& err)
	{
		GetComError(err, m_pConnection);	   	   
		return false;
	}

	return true;
}

bool S2ADOConnector::IsEOF()
{
	//try
	//{
	//	if(NULL != m_pRecordSet &&
	//		FALSE == m_pRecordSet->adPosEOF)
	//		return false;
	//}
	//catch (_com_error & err)
	//{
	//	GetComError(err, m_pConnection);	   	   
	//	return false;
	//}
	return true;
}

void S2ADOConnector::GetComError(_com_error &err, _ConnectionPtr connection)
{

	//TCHAR strFormat[ S2_STRING_COUNT ];
	//_bstr_t bstrDescription;

	//m_hrLastError = err.Error();

	//if (pConn)
	//{
	//	ErrorPtr pError;
	//	ErrorsPtr pErrors = pConn->GetErrors();

	//	pErrors->get_Item((_variant_t)((long) 0), &pError);

	//	if (pError)
	//	{
	//		_bstr_t bstrState(pError->GetSQLState());

	//		if (bstrState.length())
	//		{
	//			bstrDescription = pError->GetDescription();

	//			S2String::Format( strFormat, S2_STRING_COUNT, _T("%s,%s"), (LPCTSTR) bstrState, (LPCTSTR) bstrDescription);
	//			S2String::Copy( m_strLastError, strFormat, S2_STRING_COUNT );
	//		}

	//		pError->Release();
	//	}

	//	pErrors->Release();

	//	if( S2String::Length( m_strLastError ) )
	//	{
	//		S2TRACE( m_strLastError ); 
	//		return; 
	//	}
	//}

	//bstrDescription = err.Description();

	//_bstr_t bstrSource(err.Source());

	//S2String::Format( strFormat, S2_STRING_COUNT, _T("Error Code : %08x\n"), err.Error() );
	//S2String::Concat( m_strLastError, strFormat, S2_STRING_COUNT );

	//S2String::Format( strFormat, S2_STRING_COUNT, _T("Error Content : %s\n"), err.ErrorMessage() );
	//S2String::Concat( m_strLastError, strFormat, S2_STRING_COUNT );

	//S2String::Format( strFormat, S2_STRING_COUNT, _T("Error Source  : %s\n"), (bstrSource.length() ? (LPCTSTR) bstrSource : _T("None")) );
	//S2String::Concat( m_strLastError, strFormat, S2_STRING_COUNT );

	//S2String::Format( strFormat, S2_STRING_COUNT, _T("Error Desc  : %s\n"), (bstrDescription.length() ? (LPCTSTR) bstrDescription : _T("None")) );
	//S2String::Concat( m_strLastError, strFormat, S2_STRING_COUNT );


	//S2TRACE( m_strLastError ); 
}

int32_t S2ADOConnector::GetRecordCount()
{
	try
	{
		if (m_pRecordSet == NULL) return 0;
		return (int32_t)m_pRecordSet->GetRecordCount();
	}
	catch (_com_error& err)
	{
		GetComError(err, m_pConnection);
		return 0;
	}
}

}

#endif