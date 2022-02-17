#pragma once

#include "ClientBase.h"

// ClientBotSetting 폼 뷰입니다.

class ClientBotSetting : public ClientBase
{
	DECLARE_DYNCREATE(ClientBotSetting)

public:
	ClientBotSetting();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~ClientBotSetting();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BOT_SETTING };
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

	CIPAddressCtrl			m_ip;
	UINT					m_port;
	UINT					m_workerCount;
	UINT					m_userCount;
	long					m_startUID;
	CString					m_startId;
	CEdit					m_result;
	void					OnUpdateResult();

public:
	virtual void			OnInitialUpdate();
	afx_msg void			OnEnChangeBotSettingCount();
	afx_msg void			OnEnChangeBotSettingUid();
	afx_msg void			OnEnChangeBotSettingId();

	afx_msg void			OnBnClickedBotSettingStart();
	
	
};


