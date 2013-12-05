//
//  UnknownFriends.cpp
//  DGproto
//
//  Created by ksoo k on 2013. 12. 3..
//
//

#include "UnknownFriends.h"


void UnknownFriends::deleteById(uint64 kakaoId)
{
	for(auto iter = m_friends.begin(); iter != m_friends.end(); ++iter)
	{
		if(iter->userId == kakaoId)
		{
			m_friends.erase(iter);
			break;
		}
	}
}
void UnknownFriends::putJoinDate(int index, int64 jd)
{
	m_friends[index].joinDate = jd;
}
void UnknownFriends::putLastDate(int index, int64 jd)
{
	m_friends[index].lastDate = jd;
}
void UnknownFriends::putUserData(int index, Json::Value d)
{
	m_friends[index].userData = d;
}
UnknownFriendsData* UnknownFriends::findById(uint64 kakaoId)
{
	for(auto& i : m_friends)
	{
		if(i.userId == kakaoId)
		{
			return &i;
		}
	}
	
	return nullptr;
}

void UnknownFriends::add(const UnknownFriendsData& d)
{
	if(!findById(d.userId))
	{
		m_friends.push_back(d);
	}
}