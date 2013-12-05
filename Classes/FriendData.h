//
//  FriendData.h
//  DGproto
//
//  Created by ksoo k on 2013. 12. 5..
//
//

#pragma once

#include <string>
#include "jsoncpp/json.h"
#include <boost/format.hpp>
#include <sstream>
typedef unsigned long long uint64;
typedef long long int64;
struct FriendData
{
	int64 userId;
	std::string nick;
	int64 joinDate;
	int64 lastDate;
	std::string profileUrl;
	bool messageBlocked;
	Json::Value userData;
	std::string hashedTalkUserId;
	bool unknownFriend;
	Json::Value extraData; // 여분의 공간, 필요하면 이 공간을 쓰면 됨.
	friend std::ostream &operator<<(std::ostream &output, const FriendData &a)
	{
		std::ostringstream oss;
		oss << a.userData;
		std::ostringstream oss2;
		oss2 << a.extraData;
		auto t = boost::str(boost::format("\n\tuserId = %||\n\tnick = %||\n\tunknown = %||\n\tuserData = %||\n\t"
																			"joinDate = %||\n\tlastDate = %||\n\thashId = %||\n\tprofileUrl = %||\n\t"
																			"messageBlocked = %||\n\textraData = %||\n\t") % a.userId
												% a.nick % a.unknownFriend % oss.str() % a.joinDate  % a.lastDate %
												a.hashedTalkUserId % a.profileUrl.c_str() % a.messageBlocked % oss2.str().c_str());
		output << t << std::endl;
		return output;
	}
	FriendData()
	{
		joinDate = lastDate = 0;
		messageBlocked = false;
		unknownFriend = false;
	}
};


