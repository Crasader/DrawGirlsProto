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

int getHeartSendingRemainTime (std::string userId, int base_s)
{
	auto end = chrono::system_clock::now();
	auto currentSecond = chrono::system_clock::to_time_t(end);
	ostringstream uid;
	uid << userId;
	int ii = myDSH->getUserIntForStr("heart_" + uid.str(), 0);
	if(ii + base_s < currentSecond)
	{
		return 0;
	}
	else
		return ii + base_s - currentSecond;
	
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

std::string getRemainTimeMsg(long long remainSeconds)
{
	int days = remainSeconds / ( 60 * 60 * 24 );
	int hours = remainSeconds / ( 60 * 60 );	
	int minutes = remainSeconds / 60;
	std::string remainStr;
	if(days > 0){
		remainStr = boost::str(boost::format("%|| 일 후") % days);
	}
	else if(hours > 0){
		remainStr = boost::str(boost::format("%|| 시간 후") % hours);
	}
	else if(minutes > 0){
		remainStr = boost::str(boost::format("%|| 분 후") % minutes);
	}
	return remainStr;
}


void timeSpliter(long long time, int* year, int* month, int* day, int* hour, int* minute, int* second)
{
	if(year != nullptr)
		*year = time / 10000000000;
	time %= 10000000000;
	
	if(month != nullptr)
		*month = time / 100000000;
	time %=100000000;
	
	if(day != nullptr)
		*day = time / 1000000;
	time %= 1000000;
	
	if(hour != nullptr)
		*hour = time / 10000;
	time %= 10000;
	
	if(minute != nullptr)
		*minute = time / 100;
	time %= 100;
	
	if(second != nullptr)
		*second = time / 1;
	time %= 1;
}







