// ClientSelector.cpp : 구현 파일입니다.
//

#include "pch.h"
#include "ClientSelector.h"

// ClientSelector

IMPLEMENT_DYNCREATE(ClientSelector, ClientBase)

ClientSelector::ClientSelector()
	: ClientBase(IDD_SELECTOR)
{
}

ClientSelector::~ClientSelector()
{
}

void ClientSelector::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(ClientSelector, CFormView)
	ON_BN_CLICKED(IDC_SELECTOR_SINGLE, &ClientSelector::OnBnClickedSelectorSingle)
	ON_BN_CLICKED(IDC_SELECTOR_MULTI, &ClientSelector::OnBnClickedSelectorMulti)
END_MESSAGE_MAP()


// ClientSelector 진단입니다.

#ifdef _DEBUG
void ClientSelector::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void ClientSelector::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// ClientSelector 메시지 처리기입니다.
void ClientSelector::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}

void ClientSelector::OnBnClickedSelectorSingle()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetApp()->InitClient(COLTROLLER_TYPE::SINGLE);
}


void ClientSelector::OnBnClickedSelectorMulti()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetApp()->InitClient(COLTROLLER_TYPE::MULTI);
}
