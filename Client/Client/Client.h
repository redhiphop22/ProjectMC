
// Client.h: Client 애플리케이션의 기본 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.

class ClientBase;
class MessageProcessor;
class User;

// CClientApp:
// 이 클래스의 구현에 대해서는 Client.cpp을(를) 참조하세요.
//

class CClientApp : public CWinAppEx
{
public:
	using user_list_t = std::vector<User*>;
public:
	CClientApp() noexcept;
	virtual ~CClientApp();


// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현입니다.
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:
	void					InitClient(COLTROLLER_TYPE type);

	virtual BOOL			OnIdle(LONG lCount);
		
	bool					CreateUser(const int32_t workerCount, const bool useThread, const uint32_t ip, const uint16_t port, const int32_t userCount, const uid_t startUID, const std::string& startId, user_list_t& userList);

	user_list_t&			GetUserList()			{	return m_userList;	}

	bool					SwitchView(VIEW_PAGE viewPage);	

	ClientBase*				GetActivePage()	{	return m_ActivePage;	}

protected:
	bool					CreatePage();
	ClientBase*				GetPage(VIEW_PAGE viewPage);

private:
	ClientBase*				m_pPage[view_page_max_t] = { nullptr, };
	ClientBase*				m_ActivePage = nullptr;

	user_list_t				m_userList;		
	MessageProcessor*		m_messageProcessor = nullptr;
};

extern CClientApp theApp;
