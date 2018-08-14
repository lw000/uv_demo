#ifndef __data_h__
#define __data_h__

#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>

struct DataObject {
public:
    DataObject();
    virtual ~DataObject();
    
public:
    virtual std::string Serializable();
    virtual void UnSerializable(const std::string& json);
};

struct User : public DataObject {
	std::string uid;
	std::string name;
	std::string psd;
	std::string token;
	int status;		//0 离线 1 在线

public:
	User();
	void restore();

public:
	virtual std::string Serializable() override;
	virtual void UnSerializable(const std::string& json) override;

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
	int removeWithName(const std::string& name);
	User find(const User& user);
	User findWithUid(const std::string& uid);
	User findWithName(const std::string& name);

	bool exist(const std::string& uid);
	bool existWithName(const std::string& name);
};

extern UserMgr usermgr;
extern std::unordered_map<std::string, User> g_users;


#endif	//__data_h__
