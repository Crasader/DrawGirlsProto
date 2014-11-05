#pragma once

#ifndef __litqoo__GraphDogLib__
#define __litqoo__GraphDogLib__

#include <vector>
#include <string>
#include "cocos2d.h"
#include "jsoncpp/json.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <net/if_dl.h>
#include <ifaddrs.h>
#endif
#include <regex.h>



using namespace std;
/*
 * DES (Data Encrytion Standard) 
 *
 *    - Yu Jinyang(1998. 7. 8)
 *    - email : jinyang@writeme.com
 */

namespace GraphDogLib {
    std::string GDCreateToken(string auID,string udid,string flag,string lang,string nick,string email,string platform,string cTime,string secretKey,string dInfo);
	
		std::string trim(std::string str);
    void replaceString( std::string & strCallId, const char * pszBefore, const char * pszAfter );
   //@ string JsonObjectToString(JsonBox::Object _obj);
   //@ JsonBox::Object StringToJsonObject(string _str);
    string JsonObjectToString(Json::Value _obj);
    Json::Value StringToJsonObject(string _str);
	void JsonToLog(string tag,Json::Value _obj);

	string getLocalCode();
    void openAppStore(string appid);
	void openReview(string);
	void openUpdate(string);
	
	
	std::string IntToString (long long int number);
	void ReplaceString(std::string & strCallId, const char * pszBefore, const char * pszAfter );
	string dateFormat(string format, string datestring);
		string dateFormat(string format, long long int datestring);
	int match(const char *string, const char *pattern);
	
	std::string random_string( size_t length );
	
	
}

using namespace cocos2d;
using namespace std;
//@ typedef void (CCObject::*GDSelType)(JsonBox::Object) const;
typedef void (CCObject::*GDSelType)(Json::Value) const;


#define jsonSelType std::function<void(Json::Value)>
#define json_selector(_target,_method) std::bind(&_method,_target,std::placeholders::_1)


#define gd_selector(_SELECTOR) (GDSelType)(&_SELECTOR)


#endif

#pragma once