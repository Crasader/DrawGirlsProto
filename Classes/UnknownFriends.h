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
#include "FriendData.h"
#include "jsoncpp/json.h"


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
	
	std::vector<FriendData> m_friends;
public:
	const std::vector<FriendData>& getFriends(){return m_friends;}
	
	void deleteById(uint64 kakaoId);
	void putJoinDate(int index, int64 jd);
	void putLastDate(int index, int64 jd);
	void putUserData(int index, Json::Value d);
	void putHashedTalkUserId(int index, const std::string hashId);

	FriendData* findById(uint64 kakaoId);
	void add(const FriendData& d);
};


