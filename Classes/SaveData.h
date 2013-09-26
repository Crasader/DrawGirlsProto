//
//  SaveData.h
//  BasketWorldCup2
//
//  Created by ksoo k on 13. 1. 4..
//
//

#ifndef DrawingJack_SaveData_h
#define DrawingJack_SaveData_h

#include <map>
#include <string>
#include "cocos2d.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	#include "SaveData_ios.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	#include "SaveData_android.h"
#endif


#include "JsonBox.h"

enum SaveDataFile{
	kSDF_myDSH = 1,
	kSDF_gameInfo,
	kSDF_stageInfo,
	kSDF_cardInfo,
	kSDF_downloadedInfo
};

using namespace std;
class SaveData
{
private:
	SaveData(){
		isInit = false;
	}
	
public:
	static SaveData* sharedObject();
//	string loadDatas()
//	{
//		string t = readF();
//		return t;
//	}
	
	string getSyncKey(SaveDataFile t_sdf);
	string getSyncKey(SaveDataFile t_sdf, int i1);
	
	void setKeyValue(SaveDataFile t_sdf, string _key, string _value);
	void setKeyValue(SaveDataFile t_sdf, int i1, string _key, string _value);
	void setKeyValue(string filename, string _key, string _value);
	
	void setKeyValue(SaveDataFile t_sdf, string _key, int _value);
	void setKeyValue(SaveDataFile t_sdf, int i1, string _key, int _value);
	void setKeyValue(string filename, string _key, int _value);
	
	void setKeyValue(SaveDataFile t_sdf, string _key, double _value);
	void setKeyValue(SaveDataFile t_sdf, int i1, string _key, double _value);
	void setKeyValue(string filename, string _key, double _value);
	
	string getValue(SaveDataFile t_sdf, string _key, string _defaultValue);
	string getValue(SaveDataFile t_sdf, int i1, string _key, string _defaultValue);
	string getValue(string filename, string _key, string _defaultValue);
	
	int getValue(SaveDataFile t_sdf, string _key, int _defaultValue);
	int getValue(SaveDataFile t_sdf, int i1, string _key, int _defaultValue);
	int getValue(string filename, string _key, int _defaultValue);
	
	double getValue(SaveDataFile t_sdf, string _key, double _defaultValue);
	double getValue(SaveDataFile t_sdf, int i1, string _key, double _defaultValue);
	double getValue(string filename, string _key, double _defaultValue);
	
	void createJSON(SaveDataFile t_sdf);
	void createJSON(SaveDataFile t_sdf, int i1);
	void createJSON(string filename);
	
//	void setKeyValue(string key, string value);
//	void setKeyValue(string key, int value);
//	string getValue(string key, string defaultValue);
//	int getValue(string key, int defaultValue);
//	void createJSON();
private:
	bool isInit;
private:
//	JsonBox::Value jsonData;
	JsonBox::Value file_sync;
	map<string, bool> file_init;
};
//extern SaveData* saveData;
#endif /* defined(__BasketWorldCup2__SaveData__) */
