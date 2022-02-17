// ClientConnect.cpp : 구현 파일입니다.
//

#include "pch.h"
#include "ClientBase.h"
#include "Client.h"

// ClientBase

IMPLEMENT_DYNCREATE(ClientBase, CFormView)

ClientBase::ClientBase()
	: CFormView((UINT)0)
{
}

ClientBase::ClientBase(UINT nIDTemplate)
	: CFormView(nIDTemplate)
{
}

ClientBase::~ClientBase()
{
}

void ClientBase::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(ClientBase, CFormView)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// ClientBase 진단입니다.

#ifdef _DEBUG
void ClientBase::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void ClientBase::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

CClientApp* ClientBase::GetApp()
{
	CClientApp* app = (CClientApp*)AfxGetApp();
	return app;
}

User* ClientBase::GetUser()
{
	auto userList = GetApp()->GetUserList();
	return userList[0];
}