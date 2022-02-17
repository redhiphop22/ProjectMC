#pragma once

#include "ClientBase.h"

// ClientLogin 폼 뷰입니다.

class ClientLogin : public ClientBase
{
	DECLARE_DYNCREATE(ClientLogin)

public:
	ClientLogin();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~ClientLogin();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_LOGIN };
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

private:

public:
	virtual void			OnInitialUpdate();
};


