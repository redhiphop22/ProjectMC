#pragma once

#include "ClientBase.h"

// ClientLobby 폼 뷰입니다.

class ClientLobby : public ClientBase
{
	DECLARE_DYNCREATE(ClientLobby)

public:
	ClientLobby();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~ClientLobby();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_LOBBY };
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

public:
	virtual void			OnInitialUpdate();
	virtual void			OnEnter();

	void					SetCharacterInfo(const protocol::CHARACTER_INFO_S2C* msg);
};


