//
//  ServerDataSave.h
//  DGproto
//
//  Created by 사원3 on 13. 9. 25..
//
//

#ifndef __DGproto__ServerDataSave__
#define __DGproto__ServerDataSave__

#include "cocos2d.h"
#include "SaveData.h"

USING_NS_CC;
using namespace std;

#define mySDS ServerDataSave::sharedInstance()

#define SDS_SB mySDS->setBoolForKey
#define SDS_SI mySDS->setIntegerForKey
#define SDS_SD mySDS->setDoubleForKey
#define SDS_SS mySDS->setStringForKey
#define SDS_GB mySDS->getBoolForKey
#define SDS_GI mySDS->getIntegerForKey
#define SDS_GS mySDS->getStringForKey
#define SDS_GD mySDS->getDoubleForKey

class ServerDataSave : public CCObject
{
public:
	static ServerDataSave* sharedInstance()
	{
		static ServerDataSave* t_sds = NULL;
		if(t_sds == NULL)
		{
			t_sds = new ServerDataSave();
			t_sds->myInit();
		}
		return t_sds;
	}
	
	bool getBoolForKey(SaveDataFile f_key, string r_key)
	{
		string bool_string = myDefault->getValue(f_key, r_key, "false");
		if(bool_string == "false")		return false;
		else							return true;
	}
	bool getBoolForKey(SaveDataFile f_key, string t_key, int key_val1)
	{
		string bool_string = myDefault->getValue(f_key, CCString::createWithFormat(t_key.c_str(), key_val1)->getCString(), "false");
		if(bool_string == "false")		return false;
		else							return true;
	}
	
	bool getBoolForKey(SaveDataFile f_key, int i1, string r_key)
	{
		string bool_string = myDefault->getValue(f_key, i1, r_key, "false");
		if(bool_string == "false")		return false;
		else							return true;
	}
	bool getBoolForKey(SaveDataFile f_key, int i1, string t_key, int key_val1)
	{
		string bool_string = myDefault->getValue(f_key, i1, CCString::createWithFormat(t_key.c_str(), key_val1)->getCString(), "false");
		if(bool_string == "false")		return false;
		else							return true;
	}
	
	
	void setBoolForKey(SaveDataFile f_key, string r_key, bool t_b)
	{
		string bool_string;
		if(t_b)			bool_string = "true";
		else			bool_string = "false";
		myDefault->setKeyValue(f_key, r_key, bool_string);
	}
	void setBoolForKey(SaveDataFile f_key, string r_key, int key_val1, bool t_b)
	{
		string bool_string;
		if(t_b)			bool_string = "true";
		else			bool_string = "false";
		myDefault->setKeyValue(f_key, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), bool_string);
	}
	
	void setBoolForKey(SaveDataFile f_key, int i1, string r_key, bool t_b)
	{
		string bool_string;
		if(t_b)			bool_string = "true";
		else			bool_string = "false";
		myDefault->setKeyValue(f_key, i1, r_key, bool_string);
	}
	void setBoolForKey(SaveDataFile f_key, int i1, string r_key, int key_val1, bool t_b)
	{
		string bool_string;
		if(t_b)			bool_string = "true";
		else			bool_string = "false";
		myDefault->setKeyValue(f_key, i1, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), bool_string);
	}
	
	
	int getIntegerForKey(SaveDataFile f_key, string r_key)
	{	return myDefault->getValue(f_key, r_key, 0);	}
	int getIntegerForKey(SaveDataFile f_key, string r_key, int key_val1)
	{	return myDefault->getValue(f_key, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), 0);	}

	int getIntegerForKey(SaveDataFile f_key, int i1, string r_key)
	{	return myDefault->getValue(f_key, i1, r_key, 0);	}
	int getIntegerForKey(SaveDataFile f_key, int i1, string r_key, int key_val1)
	{	return myDefault->getValue(f_key, i1, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), 0);	}
	
	
	void setIntegerForKey(SaveDataFile f_key, string r_key, int val1)
	{	myDefault->setKeyValue(f_key, r_key, val1);		}
	void setIntegerForKey(SaveDataFile f_key, string r_key, int key_val1, int val1)
	{	myDefault->setKeyValue(f_key, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), val1);	}
	
	void setIntegerForKey(SaveDataFile f_key, int i1, string r_key, int val1)
	{	myDefault->setKeyValue(f_key, i1, r_key, val1);		}
	void setIntegerForKey(SaveDataFile f_key, int i1, string r_key, int key_val1, int val1)
	{	myDefault->setKeyValue(f_key, i1, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), val1);	}
	
	
	double getDoubleForKey(SaveDataFile f_key, string r_key)
	{	return myDefault->getValue(f_key, r_key, 0.0);	}
	double getDoubleForKey(SaveDataFile f_key, string r_key, int key_val1)
	{	return myDefault->getValue(f_key, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), 0.0);	}
	
	double getDoubleForKey(SaveDataFile f_key, int i1, string r_key)
	{	return myDefault->getValue(f_key, i1, r_key, 0.0);	}
	double getDoubleForKey(SaveDataFile f_key, int i1, string r_key, int key_val1)
	{	return myDefault->getValue(f_key, i1, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), 0.0);	}
	
	
	void setDoubleForKey(SaveDataFile f_key, string r_key, double val1)
	{	myDefault->setKeyValue(f_key, r_key, val1);		}
	void setDoubleForKey(SaveDataFile f_key, string r_key, int key_val1, double val1)
	{	myDefault->setKeyValue(f_key, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), val1);	}
	
	void setDoubleForKey(SaveDataFile f_key, int i1, string r_key, double val1)
	{	myDefault->setKeyValue(f_key, i1, r_key, val1);		}
	void setDoubleForKey(SaveDataFile f_key, int i1, string r_key, int key_val1, double val1)
	{	myDefault->setKeyValue(f_key, i1, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), val1);	}
	
	
	string getStringForKey(SaveDataFile f_key, string r_key)
	{	return myDefault->getValue(f_key, r_key, "");	}
	string getStringForKey(SaveDataFile f_key, string r_key, int key_val1)
	{	return myDefault->getValue(f_key, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), "");	}
	
	string getStringForKey(SaveDataFile f_key, int i1, string r_key)
	{	return myDefault->getValue(f_key, i1, r_key, "");	}
	string getStringForKey(SaveDataFile f_key, int i1, string r_key, int key_val1)
	{	return myDefault->getValue(f_key, i1, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), "");	}
	
	
	void setStringForKey(SaveDataFile f_key, string r_key, string val1)
	{	myDefault->setKeyValue(f_key, r_key, val1.c_str());	}
	void setStringForKey(SaveDataFile f_key, string r_key, int key_val1, string val1)
	{	myDefault->setKeyValue(f_key, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), val1.c_str());	}
	
	void setStringForKey(SaveDataFile f_key, int i1, string r_key, string val1)
	{	myDefault->setKeyValue(f_key, i1, r_key, val1.c_str());	}
	void setStringForKey(SaveDataFile f_key, int i1, string r_key, int key_val1, string val1)
	{	myDefault->setKeyValue(f_key, i1, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), val1.c_str());	}
	
private:
	SaveData* myDefault;
	
	void myInit()
	{
		myDefault = SaveData::sharedObject();
	}
};

#endif /* defined(__DGproto__ServerDataSave__) */
