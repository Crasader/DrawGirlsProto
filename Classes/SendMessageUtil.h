//
//  SendMessageUtil.h
//  DGproto
//
//  Created by ksoo k on 2013. 11. 28..
//
//

#ifndef __DGproto__SendMessageUtil__
#define __DGproto__SendMessageUtil__

#include <iostream>
#include <chrono>
#include "DataStorageHub.h"
#include "FriendData.h"
#include <string>
void setHelpSendTime( std::string userId );

int getIsNotHelpableUser( std::string userId, int base_s = 60 * 60 * 24 * 1  ); /* 1일 */

void setChallengeSendTime( std::string userId );

int getIsNotChallangableUser( std::string userId, int base_s = 60 * 60 * 24 * 1  ); /* 1일 */

void setTicketSendTime( std::string, int puzzlenumber );

int getIsNotTicketUser( std::string userId, int puzzlenumber, int base_s = 60 * 60 * 24 * 1  ); /* 1일 */

int getInviteIsSendable( std::string userId, int base_s = 60 * 60 * 24 * 31 );

void setInviteSendTime( std::string userId );


int getHeartSendingRemainTime (std::string userId, int base_s = 60 * 60 * 6);
void setHeartSendTime (std::string userId);
std::string getRemainTimeMsg(long long seconds);
void timeSpliter(long long time, int* year, int* month, int* day, int* hour, int* minute, int* second);
#endif /* defined(__DGproto__SendMessageUtil__) */
