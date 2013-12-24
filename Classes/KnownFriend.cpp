//
//  KnownFriends.cpp
//  DGproto
//
//  Created by ksoo k on 2013. 12. 3..
//
//

#include "KnownFriend.h"


void KnownFriends::deleteById(std::string kakaoId)
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
void KnownFriends::putJoinDate(int index, int64_t jd)
{
	m_friends[index].joinDate = jd;
}
void KnownFriends::putLastDate(int index, int64_t jd)
{
	m_friends[index].lastDate = jd;
}
void KnownFriends::putUserData(int index, Json::Value d)
{
	m_friends[index].userData = d;
}

void KnownFriends::putHashedTalkUserId(int index, const std::string hashId)
{
	m_friends[index].hashedTalkUserId = hashId;
}
FriendData* KnownFriends::findById(std::string kakaoId)
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

void KnownFriends::add(const FriendData& d)
{
	if(!findById(d.userId))
	{
		m_friends.push_back(d);
	}
}