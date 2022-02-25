// ClientMap.cpp : 구현 파일입니다.
//

#include "pch.h"
#include "ClientMap.h"

// ClientMap

IMPLEMENT_DYNCREATE(ClientMap, ClientBase)

ClientMap::ClientMap()
	: ClientBase(IDD_CLIENT_MAP)
{
}

ClientMap::~ClientMap()
{
	OnExit();
}

void ClientMap::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(ClientMap, CFormView)
//	ON_WM_PAINT()
//ON_WM_PAINT()
ON_WM_KEYDOWN()
ON_WM_KEYUP()
END_MESSAGE_MAP()


// ClientMap 진단입니다.

#ifdef _DEBUG
void ClientMap::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void ClientMap::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// ClientMap 메시지 처리기입니다.
void ClientMap::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}

BOOL ClientMap::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP)
	{
		KEY_MESSAGE keyMessage;
		switch(pMsg->wParam)
		{
		case 'w':	case 'W':	keyMessage = KEY_MESSAGE::W;	break;
		case 'a':	case 'A':	keyMessage = KEY_MESSAGE::A;	break;
		case 's':	case 'S':	keyMessage = KEY_MESSAGE::S;	break;
		case 'd':	case 'D':	keyMessage = KEY_MESSAGE::D;	break;
		default:
			return ClientBase::PreTranslateMessage(pMsg);
		}

		if(pMsg->message == WM_KEYDOWN)
			m_keyDown[static_cast<int32_t>(keyMessage)] = true;
		else if(pMsg->message == WM_KEYUP)
			m_keyDown[static_cast<int32_t>(keyMessage)] = false;
	}
	return ClientBase::PreTranslateMessage(pMsg);
}

void ClientMap::OnDraw(CDC* /*pDC*/)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CRect windowRecv;
	GetWindowRect(&windowRecv);

	int32_t windowX = windowRecv.Width()/2;
	int32_t windowY = windowRecv.Height()/2;

	CDC *pDC = GetDC();
	CRect position;
	for(auto& entityIter : m_entityList)
	{
		auto& entity = entityIter.second;

		int32_t x = windowX + static_cast<int>(entity->m_position.x());
		int32_t y = windowY + static_cast<int>(entity->m_position.z());
				
		position.SetRect(windowX, windowY, windowX + 30, windowY + 30);
		pDC->Ellipse(position);
		pDC->TextOutA(windowX, windowY-20, entity->m_nick.c_str());
	}
}

void ClientMap::OnEnter()
{
	Entity* entity = new Entity();

	entity->m_uid = GetUser()->m_uid;
	entity->m_nick = GetUser()->m_nick;
	entity->m_position = common::VECTOR3(0.f, 0.f, 0.f);

	m_entityList.emplace(GetUser()->m_uid, entity);
	Invalidate();
}

void ClientMap::OnExit()
{
	for(auto& entity : m_entityList)
	{
		delete entity.second;
	}
	m_entityList.clear();
}

void ClientMap::OnUpdate()
{
	time_t nowTime = s2::S2Time::Now();

	auto entityIter = m_entityList.begin();
	auto entity = entityIter->second;

	float vec[3] = {0.f, };
	bool sendPacket = false;
	if(m_keyDown[static_cast<int32_t>(KEY_MESSAGE::W)])
	{
		vec[2] = -1.f;
		sendPacket = true;
	}
	else if(m_keyDown[static_cast<int32_t>(KEY_MESSAGE::S)])
	{
		vec[2] = 1.f;
		sendPacket = true;
	}
	if(m_keyDown[static_cast<int32_t>(KEY_MESSAGE::A)])
	{
		vec[0] = -1.f;
		sendPacket = true;
	}
	else if(m_keyDown[static_cast<int32_t>(KEY_MESSAGE::D)])
	{
		vec[0] = 1.f;
		sendPacket = true;
	}

	if(sendPacket)
	{
		if(m_nextKeyEventTime < nowTime)
		{
			float length = sqrtf(powf(vec[0], 2) + powf(vec[1], 2) + powf(vec[2], 2));
			common::VECTOR3 forward(vec[0]/length, vec[1]/length, vec[2]/length);

			flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
			auto body = protocol::CreateENTITY_MOVE_VELOCITY_C2S(fbb,
				nowTime + 50,
				&entity->m_position,
				&forward,
				60.f
			);
			fbb.Finish(body);
			SEND_PACKET(ENTITY_MOVE_VELOCITY_C2S, fbb);

			m_nextKeyEventTime = nowTime + 1000;
		}
		m_sendMove = true;
	}
	else if(m_sendMove)
	{
		flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
		auto body = protocol::CreateENTITY_MOVE_STOP_C2S(fbb,
			nowTime + 50,
			&entity->m_position
		);
		fbb.Finish(body);
		SEND_PACKET(ENTITY_MOVE_STOP_C2S, fbb);

		m_nextKeyEventTime = 0;
		m_sendMove = false;
	}
}

void ClientMap::ENTITY_SPAWN_S2C(const protocol::ENTITY_SPAWN_S2C* msg)
{
	for(auto iter = msg->entity()->begin() ; iter != msg->entity()->end() ; ++iter)
	{
		auto entityIter = m_entityList.find(iter->uid());
		if(entityIter != m_entityList.end())
			continue;

		Entity* entity = new Entity();
		entity->m_uid = iter->uid();
		entity->m_nick = iter->nick_name()->data();
		entity->m_position = *iter->position();

		m_entityList.emplace(iter->uid(), entity);
	}
	Invalidate();
}

void ClientMap::ENTITY_DESTROY_S2C(const protocol::ENTITY_DESTROY_S2C* msg)
{
	uid_t uid = msg->uid();

	auto iter = m_entityList.find(uid);
	delete iter->second;
	m_entityList.erase(uid);
	Invalidate();
}

void ClientMap::ENTITY_MOVE_VELOCITY_S2C(const protocol::ENTITY_MOVE_VELOCITY_S2C* msg)
{
	auto iter = m_entityList.find(msg->uid());

	Invalidate();
}

void ClientMap::ENTITY_MOVE_STOP_S2C(const protocol::ENTITY_MOVE_STOP_S2C* msg)
{
	auto iter = m_entityList.find(msg->uid());
	iter->second->m_moveInfo.m_isMove = false;
	iter->second->m_position = *msg->position();
}

