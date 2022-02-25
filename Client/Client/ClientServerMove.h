#pragma once

#include "ClientBase.h"

// ClientServerMove 폼 뷰입니다.

class ClientServerMove : public ClientBase
{
	DECLARE_DYNCREATE(ClientServerMove)

public:
	ClientServerMove();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~ClientServerMove();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_SERVERMOVE };
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
	CString					m_uid;
	CString					m_id;

public:
	virtual void			OnInitialUpdate();
	afx_msg void			OnBnClickedServermoveMove();

	void					SERVER_CONNECT_S2C(const protocol::SERVER_CONNECT_S2C* msg);
};


