//
//  SaveData.cpp
//  BasketWorldCup2
//
//  Created by ksoo k on 13. 1. 4..
//
//

#include "SaveData.h"
#include "jsoncpp/json.h"
#include <sstream>
#include "StringCodec.h"
#include "DataStorageHub.h"

//SaveData* saveData = SaveData::sharedObject();
SaveData* SaveData::sharedObject()
{
	static SaveData* _ins = 0;
	
	if(_ins == 0)
	{
		_ins = new SaveData();
		Json::Reader reader;
		reader.parse("{}", _ins->file_sync);
//		_ins->file_sync.loadFromString("{}");
	}
	return _ins;
}

string SaveData::getSyncKey(SaveDataFile t_sdf)
{
	string return_value;
	
	if(t_sdf == kSDF_myDSH)					return_value = "KSDATA";
	else if(t_sdf == kSDF_gameInfo)			return_value = "GAMEINFO";
	else if(t_sdf == kSDF_cardInfo)			return_value = "CARDINFO";
	else if(t_sdf == kSDF_downloadedInfo)	return_value = "DOWNLOADEDINFO";
	else if(t_sdf == kSDF_achieveInfo)		return_value = "ACHIEVEINFO";
	else if(t_sdf == kSDF_log)				return_value = "LOG";
	
	return return_value;
}

string SaveData::getSyncKey(SaveDataFile t_sdf, int i1)
{
	string return_value;
	
	if(t_sdf == kSDF_stageInfo)			return_value = cocos2d::CCString::createWithFormat("STAGE%dINFO", i1)->getCString();
	else if(t_sdf == kSDF_puzzleInfo)	return_value = cocos2d::CCString::createWithFormat("PUZZLE%dINFO", i1)->getCString();
//	else if(t_sdf == kSDF_cardInfo)		return_value = cocos2d::CCString::createWithFormat("CARD%dINFO", i1)->getCString();
	
	return return_value;
}

void SaveData::createJSON(string filename)
{
	string rawData = readF(filename);
	string key = stringEncWithAES(filename);
	if(rawData == "")
	{
		Json::Reader reader;
		reader.parse("{}", file_sync[key]);
//		.loadFromString("{}");
		
		if(filename != getSyncKey(kSDF_myDSH))
		{
			int data_file_cnt = myDSH->getIntegerForKey(kDSH_Key_dataFileCnt) + 1;
			myDSH->setIntegerForKey(kDSH_Key_dataFileCnt, data_file_cnt);
			myDSH->setStringForKey(kDSH_Key_dataFileName_int1, data_file_cnt, filename);
		}
		
		CCLOG("create json : %s", filename.c_str());
	}
	else
	{
		if(filename != getSyncKey(kSDF_myDSH))
		{
			bool is_pass = false;
			int total_file = myDSH->getIntegerForKey(kDSH_Key_dataFileCnt);
			for(int i=1;!is_pass && i<=total_file;i++)
			{
				if(myDSH->getStringForKey(kDSH_Key_dataFileName_int1, i) == filename)
					is_pass = true;
			}
			
			if(!is_pass)
			{
				int data_file_cnt = myDSH->getIntegerForKey(kDSH_Key_dataFileCnt) + 1;
				myDSH->setIntegerForKey(kDSH_Key_dataFileCnt, data_file_cnt);
				myDSH->setStringForKey(kDSH_Key_dataFileName_int1, data_file_cnt, filename);
			}
		}
		
		Json::Reader reader;
		reader.parse(rawData, file_sync[key]);
	}
	file_init[filename] = true;
}
void SaveData::createJSON(SaveDataFile t_sdf){			createJSON(getSyncKey(t_sdf));		}
void SaveData::createJSON(SaveDataFile t_sdf, int i1){	createJSON(getSyncKey(t_sdf, i1));	}

void SaveData::setKeyValue(string filename, string _key, string _value, bool diskWrite /*= true*/)
{
	map<string, bool>::iterator iter;
	iter = file_init.find(filename);
	if(iter == file_init.end())
		createJSON(filename);
	
	string file_key = stringEncWithAES(filename);
	
	string key = stringEncWithAES(_key);
	string value = stringEncWithAES(_value);
	
	file_sync[file_key][key] = value;
	if(diskWrite)
	{
		Json::FastWriter writer;
		while(testF(filename, writer.write(file_sync[file_key])) <= -1){}
	}
}
void SaveData::setKeyValue(SaveDataFile t_sdf, string _key, string _value, bool diskWrite /*= true*/){			setKeyValue(getSyncKey(t_sdf), _key, _value, diskWrite);		}
void SaveData::setKeyValue(SaveDataFile t_sdf, int i1, string _key, string _value, bool diskWrite /*= true*/){	setKeyValue(getSyncKey(t_sdf, i1), _key, _value, diskWrite);	}


void SaveData::addKeyValue(string filename, string _key, string _value)
{
	map<string, bool>::iterator iter;
	iter = file_init.find(filename);
	if(iter == file_init.end())
		createJSON(filename);
	
	string file_key = stringEncWithAES(filename);
	
	string key = stringEncWithAES(_key);
	string value = stringEncWithAES(_value);
	
	(file_sync[file_key])[key] = value;
	Json::FastWriter writer;
	addF(filename, writer.write(file_sync[file_key]));
}
void SaveData::addKeyValue(SaveDataFile t_sdf, string _key, string _value)
{
	addKeyValue(getSyncKey(t_sdf), _key, _value);
}

void SaveData::resetData(SaveDataFile t_sdf)
{
	resetData(getSyncKey(t_sdf));
}

void SaveData::resetData(string filename)
{
	CCLOG("reset info file : %s", filename.c_str());
	
	map<string, bool>::iterator iter;
	iter = file_init.find(filename);
	if(iter == file_init.end())
		createJSON(filename);
	
	string file_key = stringEncWithAES(filename);
	
	Json::Reader reader;
	reader.parse("{}", file_sync[file_key]);
	while(testF(filename, "") <= -1){}
}

void SaveData::resetAllData()
{
	CCLOG("reset all info file");
	
	int total_file = myDSH->getIntegerForKey(kDSH_Key_dataFileCnt);
	for(int i=1;i<=total_file;i++)
	{
		string t_filename = myDSH->getStringForKey(kDSH_Key_dataFileName_int1, i);
		if(t_filename != "")
			resetData(t_filename);
	}
	resetData(getSyncKey(kSDF_myDSH));
	file_sync.clear();
	Json::Reader reader;
	reader.parse("{}", file_sync);
}

void SaveData::setKeyValue(string filename, string _key, int _value, bool diskWrite /*= true*/)
{
	map<string, bool>::iterator iter;
	iter = file_init.find(filename);
	if(iter == file_init.end())
		createJSON(filename);
	
	string file_key = stringEncWithAES(filename);
	
	string key = stringEncWithAES(_key);
	ostringstream valueoss;
	valueoss << _value;
	string value = stringEncWithAES(valueoss.str());
	file_sync[file_key][key] = value;
	if(diskWrite)
	{
		Json::FastWriter writer;
		while(testF(filename, writer.write(file_sync[file_key])) <= -1){}
	}
}
void SaveData::setKeyValue(SaveDataFile t_sdf, string _key, int _value, bool diskWrite /*= true*/){			setKeyValue(getSyncKey(t_sdf), _key, _value, diskWrite);		}
void SaveData::setKeyValue(SaveDataFile t_sdf, int i1, string _key, int _value, bool diskWrite /*= true*/){	setKeyValue(getSyncKey(t_sdf, i1), _key, _value, diskWrite);	}

void SaveData::setKeyValue(string filename, string _key, double _value, bool diskWrite /*= true*/)
{
	map<string, bool>::iterator iter;
	iter = file_init.find(filename);
	if(iter == file_init.end())
		createJSON(filename);
	
	string file_key = stringEncWithAES(filename);
	
	string key = stringEncWithAES(_key);
	ostringstream valueoss;
	valueoss << _value;
	string value = stringEncWithAES(valueoss.str());
	file_sync[file_key][key] = value;
	if(diskWrite)
	{
		Json::FastWriter writer;
		while(testF(filename, writer.write(file_sync[file_key])) <= -1){}
	}
}
void SaveData::setKeyValue(SaveDataFile t_sdf, string _key, double _value, bool diskWrite /*= true*/){			setKeyValue(getSyncKey(t_sdf), _key, _value, diskWrite);		}
void SaveData::setKeyValue(SaveDataFile t_sdf, int i1, string _key, double _value, bool diskWrite /*= true*/){	setKeyValue(getSyncKey(t_sdf, i1), _key, _value, diskWrite);	}

Json::Value SaveData::getFileData(string filename)
{
	Json::Value return_value;
	return_value.clear();
	
	map<string, bool>::iterator iter;
	iter = file_init.find(filename);
	if(iter == file_init.end())
		createJSON(filename);
	
	string file_key = stringEncWithAES(filename);
	
	Json::Value::Members t_member = file_sync[file_key].getMemberNames();
	while(!t_member.empty())
	{
		string t_key = t_member.back();
		string real_key = stringDecodeWithAES(t_key);
		string t_v = (file_sync[file_key])[t_key].asString();
		string t_v2 = stringDecodeWithAES(t_v);
		return_value[real_key.c_str()] = t_v2;
		t_member.pop_back();
	}
	
	return return_value;
}

void SaveData::setFileData(string filename, Json::Value t_data)
{
	map<string, bool>::iterator iter;
	iter = file_init.find(filename);
	if(iter == file_init.end())
		createJSON(filename);
	
	string file_key = stringEncWithAES(filename);
	
	Json::Value::Members t_member = t_data.getMemberNames();
	while(!t_member.empty())
	{
		string t_key = t_member.back();
		string t_value = t_data[t_key].asString();
		
		string key = stringEncWithAES(t_key);
		ostringstream valueoss;
		valueoss << t_value;
		string value = stringEncWithAES(valueoss.str());
		file_sync[file_key][key] = value;
		
		t_member.pop_back();
	}
	
//	if(diskWrite)
//	{
		Json::FastWriter writer;
		while(testF(filename, writer.write(file_sync[file_key])) <= -1){}
//	}
}

string SaveData::getValue(string filename, string _key, string _defaultValue)
{
	map<string, bool>::iterator iter;
	iter = file_init.find(filename);
	if(iter == file_init.end())
		createJSON(filename);
	
	string file_key = stringEncWithAES(filename);
	string key = stringEncWithAES(_key);
	string v = (file_sync[file_key])[key].asString();
	string v2 = stringDecodeWithAES(v);
	if(v2 == "")
		return _defaultValue;
	else
		return v2;
}
string SaveData::getValue(SaveDataFile t_sdf, string _key, string _defaultValue){			return getValue(getSyncKey(t_sdf), _key, _defaultValue);		}
string SaveData::getValue(SaveDataFile t_sdf, int i1, string _key, string _defaultValue){	return getValue(getSyncKey(t_sdf, i1), _key, _defaultValue);	}

int SaveData::getValue(string filename, string _key, int _defaultValue)
{
	map<string, bool>::iterator iter;
	iter = file_init.find(filename);
	if(iter == file_init.end())
		createJSON(filename);
	
	string file_key = stringEncWithAES(filename);
	string key = stringEncWithAES(_key);
	string v = (file_sync[file_key])[key].asString();
	string v2 = stringDecodeWithAES(v);
	int _v2 = atoi(v2.c_str());
	if(v2 == "")
		return _defaultValue;
	else
		return _v2;
}
int SaveData::getValue(SaveDataFile t_sdf, string _key, int _defaultValue){				return getValue(getSyncKey(t_sdf), _key, _defaultValue);		}
int SaveData::getValue(SaveDataFile t_sdf, int i1, string _key, int _defaultValue){		return getValue(getSyncKey(t_sdf, i1), _key, _defaultValue);	}

double SaveData::getValue(string filename, string _key, double _defaultValue)
{
	map<string, bool>::iterator iter;
	iter = file_init.find(filename);
	if(iter == file_init.end())
		createJSON(filename);
	
	string file_key = stringEncWithAES(filename);
	string key = stringEncWithAES(_key);
	string v = (file_sync[file_key])[key].asString();
	string v2 = stringDecodeWithAES(v);
	double _v2 = atof(v2.c_str());
	if(v2 == "")
		return _defaultValue;
	else
		return _v2;
}
double SaveData::getValue(SaveDataFile t_sdf, string _key, double _defaultValue){				return getValue(getSyncKey(t_sdf), _key, _defaultValue);		}
double SaveData::getValue(SaveDataFile t_sdf, int i1, string _key, double _defaultValue){		return getValue(getSyncKey(t_sdf, i1), _key, _defaultValue);	}

void SaveData::fFlush(string filename)
{
	map<string, bool>::iterator iter;
	iter = file_init.find(filename);
	if(iter == file_init.end())
		createJSON(filename);
	
	string file_key = stringEncWithAES(filename);
		
	Json::FastWriter writer;
	while(testF(filename, writer.write(file_sync[file_key])) <= -1){}
}
void SaveData::fFlush(SaveDataFile t_sdf){			fFlush(getSyncKey(t_sdf));		}
void SaveData::fFlush(SaveDataFile t_sdf, int i1){	fFlush(getSyncKey(t_sdf, i1));	}
//void SaveData::createJSON()
//{
//	string rawData = readF();
//	if(rawData == "")
//		jsonData.loadFromString("{}");
//	else
//		jsonData.loadFromString(rawData);
//	isInit = true;
//}
//void SaveData::setKeyValue(string _key, string _value)
//{
//	CCAssert(isInit == true, "call createJSON");
//	string key = stringEnc(_key);
//	string value = stringEnc(_value);
//	jsonData[key] = value;
//	ostringstream oss;
//	jsonData.writeToStream(oss);
//	testF(oss.str());
//}
//void SaveData::setKeyValue(string _key, int _value)
//{
//	CCAssert(isInit == true, "call createJSON");
//	string key = stringEnc(_key);
//	ostringstream valueoss;
//	valueoss << _value;
//	string value = stringEnc(valueoss.str());
//	jsonData[key] = value;
//	ostringstream oss;
//	jsonData.writeToStream(oss);
//	testF(oss.str());
//}
//
//string SaveData::getValue(string _key, string defaultValue)
//{
//	CCAssert(isInit == true, "call createJSON");
//	string key = stringEnc(_key);
//	string v = jsonData[key].getString();
//	string v2 = stringDecode(v);
//	if(v2 == "")
//	{
//		return defaultValue;
//	}
//	else
//	{
//		return v2;
//	}
//}
//
//int SaveData::getValue(string _key, int defaultValue)
//{
//	CCAssert(isInit == true, "call createJSON");
//	string key = stringEnc(_key);
//	string v = jsonData[key].getString();
//	string v2 = stringDecode(v);
//	int _v2 = atoi(v2.c_str());
//	if(v2 == "")
//	{
//		return defaultValue;
//	}
//	else
//	{
//		return _v2;
//	}
//}
