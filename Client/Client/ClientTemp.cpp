// ClientLogin.cpp : 구현 파일입니다.
//

#include "pch.h"
#include "ClientLogin.h"

// ClientLogin

IMPLEMENT_DYNCREATE(ClientLogin, ClientBase)

ClientLogin::ClientLogin()
	: ClientBase(IDD_CLIENT_CONNECT)
{

}

ClientLogin::~ClientLogin()
{
}

void ClientLogin::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(ClientLogin, CFormView)
END_MESSAGE_MAP()


// ClientLogin 진단입니다.

#ifdef _DEBUG
void ClientLogin::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void ClientLogin::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// ClientLogin 메시지 처리기입니다.
void ClientLogin::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}