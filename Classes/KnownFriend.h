#pragma once
#include <vector>
#include <string>
#include <map>
#include "jsoncpp/json.h"

typedef unsigned long long uint64;
typedef long long int64;
struct KnownFriendsData
{
	int64 userId;
	std::string nick;
	int64 joinDate;
	int64 lastDate;
	std::string profileUrl;
	bool messageBlocked;
	Json::Value userData;
};

class KnownFriends
{
public:
	static KnownFriends* getInstance()
	{
		static KnownFriends* t_GD = nullptr;
		if(t_GD == nullptr)
		{
			t_GD = new KnownFriends();
		}
		return t_GD;
	}
protected:
	KnownFriends() {
//		userId = joinDate = lastDate = 0;
//		messageBlocked = false;
	}
	
	std::vector<KnownFriendsData> m_friends;
public:
	const std::vector<KnownFriendsData>& getFriends(){return m_friends;}
	void deleteById(uint64 kakaoId);
	void putJoinDate(int index, int64 jd);
	void putLastDate(int index, int64 jd);
	void putUserData(int index, Json::Value d);
	KnownFriendsData* findById(uint64 kakaoId);
	void add(const KnownFriendsData& d);
};


