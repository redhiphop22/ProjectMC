#pragma once

#include "ClientBase.h"

// ClientBotRun 폼 뷰입니다.

class ClientBotRun : public ClientBase
{
	DECLARE_DYNCREATE(ClientBotRun)

public:
	ClientBotRun();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~ClientBotRun();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BOT_RUN };
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

	CListCtrl				m_userView;
	CEdit					m_notice;
public:
	virtual void			OnInitialUpdate();
	virtual void			OnEnter();
	virtual void			OnExit();

	void					OnViewUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


