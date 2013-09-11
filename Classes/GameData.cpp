//
//  GameData.cpp
//  DGproto
//
//  Created by ksoo k on 13. 9. 6..
//
//

#include "GameData.h"

float deg2Rad(float x) { return x * M_PI / 180.f;}
float rad2Deg(float x) { return x * 180.f / M_PI;}

CCPoint ip2ccp(const IntPoint& ip)
{
	return CCPoint((ip.x - 1) * pixelSize + 1.f,
				   (ip.y - 1) * pixelSize + 1.f);
}

IntPoint ccp2ip(const CCPoint& cc)
{
	IntPoint ip;
	ip.x = round((cc.x - 1) / pixelSize + 1);
	ip.y = round((cc.y - 1) / pixelSize + 1);
	return ip;
}

GameData* gameData = GameData::sharedGameData();