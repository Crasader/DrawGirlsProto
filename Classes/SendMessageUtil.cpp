//
//  SendMessageUtil.cpp
//  DGproto
//
//  Created by ksoo k on 2013. 11. 28..
//
//

#include "SendMessageUtil.h"


void setHelpSendTime( std::string userId )
{
	auto end = chrono::system_clock::now();
	auto currentSecond = chrono::system_clock::to_time_t(end);
	ostringstream oss;
	oss << userId;
	
	myDSH->setUserIntForStr("help_" + oss.str(), currentSecond);
}


int getIsNotHelpableUser( std::string userId, int base_s) /* 1일 */
{
	auto end = chrono::system_clock::now();
	auto currentSecond = chrono::system_clock::to_time_t(end);
	ostringstream oss;
	oss << userId;
	int ii = myDSH->getUserIntForStr("help_" + oss.str(), 0);
	if(ii + base_s < currentSecond) // 보낼 수 있다.
	{
		return 0;
	}
	else
	{
		return ii + base_s - currentSecond; // 남은 시간 리턴
	}
}


void setChallengeSendTime( std::string userId )
{
	auto end = chrono::system_clock::now();
	auto currentSecond = chrono::system_clock::to_time_t(end);
	ostringstream oss;
	oss << userId;
	myDSH->setUserIntForStr("challenge_" + oss.str(), currentSecond);
}

int getIsNotChallangableUser( std::string userId, int base_s)
{
	auto end = chrono::system_clock::now();
	auto currentSecond = chrono::system_clock::to_time_t(end);
	ostringstream oss;
	oss << userId;
	int ii = myDSH->getUserIntForStr("challenge_" + oss.str(), 0);
	if(ii + base_s < currentSecond) // 보낼 수 있다.
	{
		return 0;
	}
	else
	{
		return ii + base_s - currentSecond; // 남은 시간 리턴
	}
}

void setTicketSendTime( std::string userId, int puzzlenumber )
{
	auto end = chrono::system_clock::now();
	auto currentSecond = chrono::system_clock::to_time_t(end);
	ostringstream oss;
	oss << puzzlenumber;
	// ticket_2_yourid 형태.
	ostringstream uid;
	uid << userId;
	myDSH->setUserIntForStr(std::string("ticket_") + oss.str() + "_" + uid.str(), currentSecond);
}

int getIsNotTicketUser( std::string userId, int puzzlenumber, int base_s)
{
	auto end = chrono::system_clock::now();
	auto currentSecond = chrono::system_clock::to_time_t(end);
	ostringstream oss;
	oss << puzzlenumber;
	ostringstream uid;
	uid << userId;
	// ticket_2_yourid 형태.
	int ii = myDSH->getUserIntForStr(std::string("ticket_") + oss.str() + "_" + uid.str(), 0);
	if(ii + base_s < currentSecond) // 보낼 수 있다.
	{
		return 0;
	}
	else
	{
		return ii + base_s - currentSecond; // 남은 시간 리턴
	}
}


int getInviteIsSendable( std::string userId, int base_s /*= 60 * 60 * 24 * 31*/ ) /* 31¿œ. */
{
	auto end = chrono::system_clock::now();
	auto currentSecond = chrono::system_clock::to_time_t(end);
	ostringstream uid;
	uid << userId;
	int ii = myDSH->getUserIntForStr("invite_" + uid.str(), 0);
	if(ii + base_s < currentSecond)
	{
		return 1;
	}
	else
		return 0;
	
	//		if(ii + base_s < GameSystem::getCurrentTime_s())
	//		{
	//			return 1;
	//		}
	//		else
	//			return 0;
}

void setInviteSendTime( std::string userId )
{
	auto end = chrono::system_clock::now();
	auto currentSecond = chrono::system_clock::to_time_t(end);
	ostringstream uid;
	uid << userId;
	myDSH->setUserIntForStr("invite_" + uid.str(), currentSecond);
	myDSH->setUserIntForStr("invitecount", myDSH->getUserIntForStr("invitecount", 0) + 1);
	//		saveData->setKeyValue(fbid, GameSystem::getCurrentTime_s());
}

int getHeartIsSendable (std::string userId, int base_s)
{
	auto end = chrono::system_clock::now();
	auto currentSecond = chrono::system_clock::to_time_t(end);
	ostringstream uid;
	uid << userId;
	int ii = myDSH->getUserIntForStr("heart_" + uid.str(), 0);
	if(ii + base_s < currentSecond)
	{
		return 1;
	}
	else
		return 0;
	
	//		if(ii + base_s < GameSystem::getCurrentTime_s())
	//		{
	//			return 1;
	//		}
	//		else
	//			return 0;
}
void setHeartSendTime (std::string userId)
{
	auto end = chrono::system_clock::now();
	auto currentSecond = chrono::system_clock::to_time_t(end);
	ostringstream uid;
	uid << userId;
	myDSH->setUserIntForStr("heart_" + uid.str(), currentSecond);
	//		saveData->setKeyValue(fbid, GameSystem::getCurrentTime_s());
}
