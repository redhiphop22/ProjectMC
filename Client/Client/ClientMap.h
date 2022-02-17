#pragma once

#include "ClientBase.h"

// ClientMap 폼 뷰입니다.

class ClientMap : public ClientBase
{
	class Entity
	{
		uid_t				m_uid;
		std::string			m_nick;
	};

	using entity_list_t = std::unordered_map<uid_t, Entity*>;

	DECLARE_DYNCREATE(ClientMap)

public:
	ClientMap();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~ClientMap();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_MAP };
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

public:
	virtual void			OnInitialUpdate();
	virtual void			OnExit();
	virtual void			OnUpdate();

	void					ENTITY_SPAWN_S2C(const protocol::ENTITY_SPAWN_S2C* msg);	
	void					ENTITY_DESTROY_S2C(const protocol::ENTITY_DESTROY_S2C* msg);

private:
	entity_list_t			m_entityList;
};


