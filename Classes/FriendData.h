//
//  FriendData.h
//  DGproto
//
//  Created by 사원3 on 13. 10. 16..
//
//

#ifndef __DGproto__FriendData__
#define __DGproto__FriendData__

#include "cocos2d.h"
#include <map>

USING_NS_CC;
using namespace std;

class FD_person
{
public:
	string friend_nickname;
	string hashed_talk_user_id;
	bool message_blocked;
	string nickname;
	string profile_image_url;
	bool supported_device;
	string user_id;
	
	FD_person() : supported_device(true)
	{
		
	}
};

class FriendData : public CCObject
{
public:
	static FriendData* sharedInstance()
	{
		static FriendData* t_fd = NULL;
		if(!t_fd)
		{
			t_fd = new FriendData();
			t_fd->myInit();
		}
		
		return t_fd;
	}
	
private:
	map<string, FD_person> app_friends_info;
	unsigned int friends_count;
	map<string, FD_person> friends_info;
	
	void myInit()
	{
		
	}
};

#endif /* defined(__DGproto__FriendData__) */
