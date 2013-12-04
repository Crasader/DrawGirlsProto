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

void UnknownFriends::addById(uint64 kakaoId)
{
	if(!findById(kakaoId))
	{
		UnknownFriendsData ufd;
		ufd.userId = kakaoId;
		m_friends.push_back(ufd);
	}
}