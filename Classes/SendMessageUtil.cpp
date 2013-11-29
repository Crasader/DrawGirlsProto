//
//  SendMessageUtil.cpp
//  DGproto
//
//  Created by ksoo k on 2013. 11. 28..
//
//

#include "SendMessageUtil.h"


void setHelpSendTime( string userId )
{
	auto end = chrono::system_clock::now();
	auto currentSecond = chrono::system_clock::to_time_t(end);
	myDSH->setUserIntForStr("help_" + userId, currentSecond);
}


int getIsNotHelpableUser( std::string userId, int base_s) /* 1일 */
{
	auto end = chrono::system_clock::now();
	auto currentSecond = chrono::system_clock::to_time_t(end);
	
	int ii = myDSH->getUserIntForStr("help_" + userId, 0);
	if(ii + base_s < currentSecond) // 보낼 수 있다.
	{
		return 0;
	}
	else
	{
		return ii + base_s - currentSecond; // 남은 시간 리턴
	}
	//		if(ii + base_s < GameSystem::getCurrentTime_s())
	//		{
	//			return 1;
	//		}
	//		else
	//			return 0;
}
