// ClientBotSetting.cpp : 구현 파일입니다.
//

#include "pch.h"
#include "ClientBotSetting.h"

// ClientBotSetting

IMPLEMENT_DYNCREATE(ClientBotSetting, ClientBase)

ClientBotSetting::ClientBotSetting()
	: ClientBase(IDD_BOT_SETTING)
	, m_port(36101)
	, m_workerCount(6)
	, m_userCount(16)
	, m_startUID(1)
	, m_startId(_T("Bot"))
{
}

ClientBotSetting::~ClientBotSetting()
{
}

void ClientBotSetting::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BOT_SETTING_IP, m_ip);
	DDX_Text(pDX, IDC_BOT_SETTING_PORT, m_port);
	DDV_MinMaxUInt(pDX, m_port, 0, 65535);
	DDX_Text(pDX, IDC_BOT_SETTING_WORKER, m_workerCount);
	DDX_Text(pDX, IDC_BOT_SETTING_COUNT, m_userCount);
	DDV_MinMaxUInt(pDX, m_userCount, 0, 999);
	DDX_Text(pDX, IDC_BOT_SETTING_UID, m_startUID);
	DDX_Text(pDX, IDC_BOT_SETTING_ID, m_startId);
	DDX_Control(pDX, IDC_BOT_SETTING_RESULT, m_result);
}

BEGIN_MESSAGE_MAP(ClientBotSetting, CFormView)
	ON_EN_CHANGE(IDC_BOT_SETTING_COUNT, &ClientBotSetting::OnEnChangeBotSettingCount)
	ON_EN_CHANGE(IDC_BOT_SETTING_UID, &ClientBotSetting::OnEnChangeBotSettingUid)
	ON_EN_CHANGE(IDC_BOT_SETTING_ID, &ClientBotSetting::OnEnChangeBotSettingId)
	ON_BN_CLICKED(IDC_BOT_SETTING_START, &ClientBotSetting::OnBnClickedBotSettingStart)	
END_MESSAGE_MAP()


// ClientBotSetting 진단입니다.

#ifdef _DEBUG
void ClientBotSetting::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void ClientBotSetting::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// ClientBotSetting 메시지 처리기입니다.
void ClientBotSetting::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_ip.SetAddress(127, 0, 0, 1);
}

void ClientBotSetting::OnUpdateResult()
{
	UpdateData(TRUE);

	if(m_startId.IsEmpty())	return;

	char idNum[6];
	sprintf_s(idNum, 6, "%05d", static_cast<int32_t>(m_startUID));
	CString startId = m_startId + idNum;
	sprintf_s(idNum, 6, "%05d", static_cast<int32_t>(m_startUID+m_userCount-1));
	CString endId = m_startId + idNum;
	m_result.SetWindowTextA(startId + " ~ " + endId);
}

void ClientBotSetting::OnEnChangeBotSettingCount()
{
	OnUpdateResult();
}

void ClientBotSetting::OnEnChangeBotSettingUid()
{
	OnUpdateResult();
}

void ClientBotSetting::OnEnChangeBotSettingId()
{
	OnUpdateResult();
}

void ClientBotSetting::OnBnClickedBotSettingStart()
{
	UpdateData(TRUE);

	BYTE bIP[ 4 ] ;
	m_ip.GetAddress( bIP[0], bIP[1], bIP[2], bIP[3] );
	UINT32 ip = bIP[0] | (bIP[1] << 8) | (bIP[2] << 16) | (bIP[3] << 24);

	CClientApp::user_list_t userList;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetApp()->CreateUser(m_workerCount, true, ip, m_port, m_userCount, m_startUID, m_startId.GetBuffer(), userList);
	GetApp()->SwitchView(VIEW_PAGE::BOT_RUN);
}
