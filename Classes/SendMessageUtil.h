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


void setHelpSendTime( string userId );

int getIsNotHelpableUser( std::string userId, int base_s = 60 * 60 * 24 * 1  ); /* 1일 */

#endif /* defined(__DGproto__SendMessageUtil__) */
