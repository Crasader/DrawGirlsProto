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

typedef unsigned long long uint64;

struct UnknownFriendsData
{
	uint64 userId;
	std::string nick;
	uint64 joinDate;
	uint64 lastDate;
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
	UnknownFriendsData* findById(uint64 kakaoId);
	void add(const UnknownFriendsData& d);
};


