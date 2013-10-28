//
//  LogData.h
//  DGproto
//
//  Created by 사원3 on 13. 10. 28..
//
//

#ifndef __DGproto__LogData__
#define __DGproto__LogData__

#include "cocos2d.h"
#include "SaveData.h"
#include "hspConnector.h"

USING_NS_CC;
using namespace std;

#define myLog LogData::sharedInstance()

enum LOG_KEY
{
	kLOG_count = 0,
	kLOG_data,
	kLOG_get_percent,
	kLOG_die_missileToLine,
	kLOG_die_shockwave,
	kLOG_die_other,
	kLOG_show_fever
};

class LogData : public CCObject
{
public:
	static LogData* sharedInstance()
	{
		static LogData* t_log = NULL;
		if(t_log == NULL)
		{
			t_log = new LogData();
			t_log->myInit();
		}
		return t_log;
	}
	
	int getLogCount()
	{
		return myDefault->getValue(kSDF_log, getRKey(kLOG_count), 0);
	}
	
	void addLog(LOG_KEY t_key, int t_time)
	{
		int log_count = getLogCount();
		log_count++;
		
		myDefault->addKeyValue(kSDF_log, CCString::createWithFormat(getRKey(kLOG_data).c_str(), log_count)->getCString(), CCString::createWithFormat(getRKey(t_key).c_str(), t_time)->getCString());
		myDefault->setKeyValue(kSDF_log, getRKey(kLOG_count), log_count);
	}
	void addLog(LOG_KEY t_key, int t_time, float t_percent)
	{
		int log_count = getLogCount();
		log_count++;
		
		myDefault->addKeyValue(kSDF_log, CCString::createWithFormat(getRKey(kLOG_data).c_str(), log_count)->getCString(), CCString::createWithFormat(getRKey(t_key).c_str(), t_time, t_percent)->getCString());
		myDefault->setKeyValue(kSDF_log, getRKey(kLOG_count), log_count);
	}
	
	void sendLog(string t_category)
	{
		Json::Value param;
		
		param["memberNo"] = hspConnector::get()->getHSPMemberNo();
		
		Json::Value contents;
		for(int i=0;i<getLogCount();i++)
			contents[i] = getLog(kLOG_data, i);
		
		param["content"] = contents;
		param["category"] = t_category.c_str();
		
		hspConnector::get()->command("writelog", param, NULL);
		myDefault->resetData(kSDF_log);
	}
	
private:
	SaveData* myDefault;
	
	string getLog(LOG_KEY t_key, int t_i)
	{
		return myDefault->getValue(kSDF_log, CCString::createWithFormat(getRKey(kLOG_data).c_str(), t_i)->getCString(), "");
	}
	
	string getRKey(LOG_KEY t_key)
	{
		string rv;
		
		if(t_key == kLOG_count)						rv = "count";
		else if(t_key == kLOG_data)					rv = "data%d";
		else if(t_key == kLOG_get_percent)			rv = "time_%d_get_percent%.3f";
		else if(t_key == kLOG_die_missileToLine)	rv = "time_%d_die_missileToLine";
		else if(t_key == kLOG_die_shockwave)		rv = "time_%d_die_shockwave";
		else if(t_key == kLOG_die_other)			rv = "time_%d_die_other";
		else if(t_key == kLOG_show_fever)			rv = "time_%d_show_fever";
		
		return rv.c_str();
	}
	
	void myInit()
	{
		myDefault = SaveData::sharedObject();
	}
};

#endif /* defined(__DGproto__LogData__) */
