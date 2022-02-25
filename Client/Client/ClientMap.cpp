// ClientMap.cpp : ���� �����Դϴ�.
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
END_MESSAGE_MAP()


// ClientMap �����Դϴ�.

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


// ClientMap �޽��� ó�����Դϴ�.
void ClientMap::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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

}

void ClientMap::ENTITY_SPAWN_S2C(const protocol::ENTITY_SPAWN_S2C* msg)
{
	for(auto iter = msg->entity()->begin() ; iter != msg->entity()->end() ; ++iter)
	{
		auto entityIter = m_entityList.find(iter->uid());
		if(entityIter != m_entityList.end())
			continue;

		Entity* entity = new Entity();
		m_entityList.emplace(iter->uid(), entity);
	}
}

void ClientMap::ENTITY_DESTROY_S2C(const protocol::ENTITY_DESTROY_S2C* msg)
{
	uid_t uid = msg->uid();

	auto iter = m_entityList.find(uid);
	delete iter->second;
	m_entityList.erase(uid);
}