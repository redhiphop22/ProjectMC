#pragma once

#include "resource.h"
#include "ControllerSingle.h"
#include "Client.h"
#include "User.h"

#define SEND_PACKET(p, f)	CONTROLLER_S.p(GetUser(), f)

class ClientBase : public CFormView
{
	DECLARE_DYNCREATE(ClientBase)

public:
	ClientBase();
	ClientBase(UINT nIDTemplate);           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~ClientBase();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	CClientApp*		GetApp();
	User*			GetUser();

public:
	virtual void	OnEnter() {};
	virtual void	OnExit() {};
	virtual void	OnUpdate() {};
};
