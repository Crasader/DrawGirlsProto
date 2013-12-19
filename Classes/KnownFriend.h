#pragma once
#include <vector>
#include <string>
#include <map>
#include "FriendData.h"
#include "jsoncpp/json.h"



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
	
	std::vector<FriendData> m_friends;
public:
	const std::vector<FriendData>& getFriends(){return m_friends;}
	void deleteById(std::string kakaoId);
	void putJoinDate(int index, int64_t jd);
	void putLastDate(int index, int64_t jd);
	void putUserData(int index, Json::Value d);
	void putHashedTalkUserId(int index, const std::string hashId);
	FriendData* findById(std::string kakaoId);
	void add(const FriendData& d);
};


