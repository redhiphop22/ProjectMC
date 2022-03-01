#pragma once

#include "ClientBase.h"

// ClientMap �� ���Դϴ�.

class ClientMap : public ClientBase
{
	enum class KEY_MESSAGE
	{
		W,
		A,
		S,
		D,

		MAX
	};

	struct Entity
	{
		struct MoveInfo
		{
			bool			m_isMove;
			time_t			m_excuteTime;
			common::VECTOR3 m_position;
			float			m_speed;
		};

		uid_t				m_uid;
		std::string			m_nick;

		common::VECTOR3		m_position;
		MoveInfo			m_moveInfo;
	};	

	using entity_list_t = std::unordered_map<uid_t, Entity*>;

	DECLARE_DYNCREATE(ClientMap)

public:
	ClientMap();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	virtual void			OnInitialUpdate();
	virtual BOOL			PreTranslateMessage(MSG* pMsg);
	virtual void			OnDraw(CDC* /*pDC*/);

	virtual void			OnEnter();
	virtual void			OnExit();
	virtual void			OnUpdate();

	void					ENTITY_SPAWN_S2C(const protocol::ENTITY_SPAWN_S2C* msg);	
	void					ENTITY_DESTROY_S2C(const protocol::ENTITY_DESTROY_S2C* msg);
	void					ENTITY_MOVE_VELOCITY_S2C(const protocol::ENTITY_MOVE_VELOCITY_S2C* msg);
	void					ENTITY_MOVE_STOP_S2C(const protocol::ENTITY_MOVE_STOP_S2C* msg);

private:
	entity_list_t			m_entityList;
	bool					m_keyDown[static_cast<int32_t>(KEY_MESSAGE::MAX)] = {false, };
	bool					m_sendMove = false;
	time_t					m_nextKeyEventTime = 0;
public:
	
	
	
};


