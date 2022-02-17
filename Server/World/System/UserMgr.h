#pragma once

class IocpSession;
class UserInfo;

class UserMgr
{
	using connect_watting_list_t = std::unordered_map<uid_t, IocpSession*>;
	using user_list_t = std::unordered_map<uid_t, UserInfo*>;

public:
	UserMgr();
	~UserMgr();

	bool					Create();
	void					Destroy();

	void					OnUpdate();

	bool					InsertWaittingUser(uid_t uid, std::string& id, IocpSession* session);
	bool					DeleteWaittingUser(uid_t uid);

	bool					InsertUser(const protocol_svr::SERVER_CONNECT_DB_ACK* msg);

	UserInfo*				GetUserInfo(uid_t uid);

private:
	connect_watting_list_t	m_wattingUserList;
	user_list_t				m_userList;
};