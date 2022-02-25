// ClientServerMove.cpp : 구현 파일입니다.
//

#include "pch.h"
#include "ClientServerMove.h"

// ClientServerMove

IMPLEMENT_DYNCREATE(ClientServerMove, ClientBase)

ClientServerMove::ClientServerMove()
	: ClientBase(IDD_CLIENT_SERVERMOVE)
	, m_uid(_T("1"))
	, m_id(_T("test1"))
{
}

ClientServerMove::~ClientServerMove()
{
}

void ClientServerMove::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SERVERMOVE_UID, m_uid);
	DDX_Text(pDX, IDC_SERVERMOVE_ID, m_id);
}

BEGIN_MESSAGE_MAP(ClientServerMove, CFormView)
	ON_BN_CLICKED(IDC_SERVERMOVE_MOVE, &ClientServerMove::OnBnClickedServermoveMove)
END_MESSAGE_MAP()


// ClientServerMove 진단입니다.

#ifdef _DEBUG
void ClientServerMove::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void ClientServerMove::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// ClientServerMove 메시지 처리기입니다.
void ClientServerMove::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}

void ClientServerMove::OnBnClickedServermoveMove()
{
	UpdateData(TRUE);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
	auto body = protocol::CreateSERVER_CONNECT_C2S(fbb,
		atoll(m_uid.GetBuffer()),
		fbb.CreateString(m_id.GetBuffer())
	);
	fbb.Finish(body);

	SEND_PACKET(SERVER_CONNECT_C2S, fbb);
}

void ClientServerMove::SERVER_CONNECT_S2C(const protocol::SERVER_CONNECT_S2C* msg)
{
	auto user = GetUser();
	user->m_uid = msg->uid();
}