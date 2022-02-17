#include "World.h"
#include "UserMgr.h"
#include "UserInfo.h"
#include "IocpSession.h"

UserMgr::UserMgr()
{
}

UserMgr::~UserMgr()
{
}

bool UserMgr::Create()
{
	return true;
}

void UserMgr::Destroy()
{
	for(auto& userList : m_userList)
	{
		UserInfo* userInfo = userList.second;
		SAFE_DELETE(userInfo);
	}
}

void UserMgr::OnUpdate()
{
	std::vector<uid_t> removeList;
	for(auto& userList : m_userList)
	{
		auto* session = userList.second->GetSession();
		if(false == session->IsActive())
		{
			removeList.push_back(userList.first);
		}
	}

	for(auto& remove : removeList)
	{
		auto userInfo = m_userList.find(remove);
		if(userInfo == m_userList.end())
		{
			continue;
		}
		userInfo->second->Destroy();
		m_userList.erase(remove);
	}
}

bool UserMgr::InsertWaittingUser(uid_t uid, std::string& id, IocpSession* session)
{
	auto userIter = m_userList.find(uid);
	if(userIter != m_userList.end())
	{
		// 중복처리
		// 둘다 접속 종료
		return true;
	}
	auto wattingIter = m_wattingUserList.find(uid);
	if(wattingIter != m_wattingUserList.end())
	{
		// 로그인 대기중 패킷 버림
		return true;
	}

	m_wattingUserList.emplace(uid, session);

	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
	auto body = protocol_svr::CreateSERVER_CONNECT_DB_REQ(fbb, 
		uid,
		fbb.CreateString(id.c_str())
	);
	fbb.Finish(body);
	
	return MESSAGE_PROCESSOR().SnedPacket_DB(0, 0, protocol_svr::MESSAGE_SERVER_CONNECT_DB_REQ, fbb);
}

bool UserMgr::DeleteWaittingUser(uid_t uid)
{
	auto user = m_wattingUserList.find(uid);
	if(user == m_wattingUserList.end())
	{
		return false;
	}
	m_wattingUserList.erase(uid);

	return true;
}

bool UserMgr::InsertUser(const protocol_svr::SERVER_CONNECT_DB_ACK* msg)
{
	uid_t uid = msg->uid();
	
	auto wattingIter = m_wattingUserList.find(uid);
	if(wattingIter == m_wattingUserList.end())
	{
		// 망함 리턴처리
		//SERVER_CONNECT_C2S
		return true;
	}

	auto session = wattingIter->second;
	m_wattingUserList.erase(uid);

	if(session->IsActive())
	{
		UserInfo* userInfo = new UserInfo(session, uid, msg->authority());
		if(nullptr == userInfo)
		{
			// 망함 리턴처리
			return true;
		}
		auto result = m_userList.emplace(uid, userInfo);
		if(false == result.second)
		{
			// 망함 리턴처리
			return true;
		}

		flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
		auto body = protocol::CreateSERVER_CONNECT_S2C(fbb, 
			common::RESULT_CODE::RESULT_CODE_SUCESS,
			uid
		);
		fbb.Finish(body);

		userInfo->SendPacket(protocol::MESSAGE_SERVER_CONNECT_S2C, fbb);
	}
	
	return true;
}

UserInfo* UserMgr::GetUserInfo(uid_t uid)
{
	auto user = m_userList.find(uid);
	if(user == m_userList.end())
	{
		return nullptr;
	}

	return user->second;
}