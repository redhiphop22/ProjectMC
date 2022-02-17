#pragma once

#include "ClientBase.h"

// ClientConnect 폼 뷰입니다.

class ClientConnect : public ClientBase
{
	DECLARE_DYNCREATE(ClientConnect)

public:
	ClientConnect();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~ClientConnect();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_CONNECT };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:

	CIPAddressCtrl			m_ip;
	UINT					m_port;

public:
	virtual void			OnInitialUpdate();
	afx_msg void			OnBnClickedConnectConnect();
};


