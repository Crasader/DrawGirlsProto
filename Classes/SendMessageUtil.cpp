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
}


void setChallengeSendTime( string userId )
{
	auto end = chrono::system_clock::now();
	auto currentSecond = chrono::system_clock::to_time_t(end);
	myDSH->setUserIntForStr("challenge_" + userId, currentSecond);
}

int getIsNotChallangableUser( std::string userId, int base_s)
{
	auto end = chrono::system_clock::now();
	auto currentSecond = chrono::system_clock::to_time_t(end);
	
	int ii = myDSH->getUserIntForStr("challenge_" + userId, 0);
	if(ii + base_s < currentSecond) // 보낼 수 있다.
	{
		return 0;
	}
	else
	{
		return ii + base_s - currentSecond; // 남은 시간 리턴
	}
}

void setTicketSendTime( string userId, int puzzlenumber )
{
	auto end = chrono::system_clock::now();
	auto currentSecond = chrono::system_clock::to_time_t(end);
	ostringstream oss;
	oss << puzzlenumber;
	// ticket_2_yourid 형태.
	myDSH->setUserIntForStr(std::string("ticket_") + oss.str() + "_" + userId, currentSecond);
}

int getIsNotTicketUser( std::string userId, int puzzlenumber, int base_s)
{
	auto end = chrono::system_clock::now();
	auto currentSecond = chrono::system_clock::to_time_t(end);
	ostringstream oss;
	oss << puzzlenumber;
	// ticket_2_yourid 형태.
	int ii = myDSH->getUserIntForStr(std::string("ticket_") + oss.str() + "_" + userId, 0);
	if(ii + base_s < currentSecond) // 보낼 수 있다.
	{
		return 0;
	}
	else
	{
		return ii + base_s - currentSecond; // 남은 시간 리턴
	}
}