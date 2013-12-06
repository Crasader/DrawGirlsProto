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

void UnknownFriends::putHashedTalkUserId(int index, const std::string hashId)
{
	m_friends[index].hashedTalkUserId = hashId;
}
FriendData* UnknownFriends::findById(uint64 kakaoId)
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

void UnknownFriends::add(const FriendData& d)
{
	if(!findById(d.userId))
	{
		m_friends.push_back(d);
	}
}