//
//  UnknownFriends.h
//  DGproto
//
//  Created by ksoo k on 2013. 12. 3..
//
//
#pragma once
#include <vector>
#include <string>
#include <map>
#include "jsoncpp/json.h"
typedef unsigned long long uint64;
typedef long long int64;

struct UnknownFriendsData
{
	int64 userId;
	std::string nick;
	int64 joinDate;
	int64 lastDate;
	std::string profileUrl;
	bool messageBlocked;
	Json::Value userData;
};

class UnknownFriends
{
public:
	static UnknownFriends* getInstance()
	{
		static UnknownFriends* t_GD = nullptr;
		if(t_GD == nullptr)
		{
			t_GD = new UnknownFriends();
		}
		return t_GD;
	}
protected:
	UnknownFriends() {}
	
	std::vector<UnknownFriendsData> m_friends;
public:
	const std::vector<UnknownFriendsData>& getFriends(){return m_friends;}
	
	void deleteById(uint64 kakaoId);
	void putJoinDate(int index, int64 jd);
	void putLastDate(int index, int64 jd);
	void putUserData(int index, Json::Value d);
	UnknownFriendsData* findById(uint64 kakaoId);
	void add(const UnknownFriendsData& d);
};


