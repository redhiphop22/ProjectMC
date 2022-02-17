// ClientLogin.cpp : 구현 파일입니다.
//

#include "pch.h"
#include "ClientLogin.h"

// ClientLogin

IMPLEMENT_DYNCREATE(ClientLogin, ClientBase)

ClientLogin::ClientLogin()
	: ClientBase(IDD_CLIENT_LOGIN)
	, m_id(_T("idtest"))
	, m_pw(_T("pwtest"))
{
}

ClientLogin::~ClientLogin()
{
}

void ClientLogin::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LOGIN_ID, m_id);
	DDX_Text(pDX, IDC_LOGIN_PW, m_pw);
}

BEGIN_MESSAGE_MAP(ClientLogin, CFormView)
	ON_BN_CLICKED(IDC_LOGIN_LOGIN, &ClientLogin::OnBnClickedLoginLogin)
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

void ClientLogin::OnBnClickedLoginLogin()
{
	UpdateData(TRUE);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
	auto body = protocol::CreateLOGIN_C2S(fbb,
		fbb.CreateString(m_id.GetBuffer()),
		fbb.CreateString(m_pw.GetBuffer())
	);
	fbb.Finish(body);
	
	SEND_PACKET(LOGIN_C2S, fbb);
}
