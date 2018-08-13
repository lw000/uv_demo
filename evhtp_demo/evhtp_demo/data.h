#ifndef __data_h__
#define __data_h__

#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>

struct User {
	std::string uid;
	std::string uname;
	std::string psd;
	std::string token;
	int status;		//0 离线 1 在线

public:
	User();
	void restore();

public:
	std::string Serializable();
	void UnSerializable(const std::string& json);

public:
	bool operator ==(const User& user);
};

class UserMgr {
private:
	std::mutex m;

private:
	std::vector<User> users;

public:
	UserMgr();

public:
	int add(const User& user);
	int modify(const User& user);
	int remove(const User& user);
	int removeWithUid(const std::string& uid);
	int removeWithUname(const std::string& uname);
	User find(const User& user);
	User findWithUid(const std::string& uid);
	User findWithUname(const std::string& uname);

	bool exist(const std::string& uid);
	bool existWithUname(const std::string& uname);
};

extern UserMgr usermgr;
extern std::unordered_map<std::string, User> g_users;


#endif	//__data_h__
