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
	else if(t_sdf == kSDF_log)				return_value = "LOG";
	
	return return_value;
}

string SaveData::getSyncKey(SaveDataFile t_sdf, int i1)
{
	string return_value;
	
	if(t_sdf == kSDF_stageInfo)			return_value = cocos2d::CCString::createWithFormat("STAGE%dINFO", i1)->getCString();
	else if(t_sdf == kSDF_puzzleInfo)	return_value = cocos2d::CCString::createWithFormat("PUZZLE%dINFO", i1)->getCString();
	
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
		cocos2d::CCLog("create json : %s", filename.c_str());
	}
	else
	{
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
		testF(filename, writer.write(file_sync[file_key]));
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
	map<string, bool>::iterator iter;
	iter = file_init.find(filename);
	if(iter == file_init.end())
		createJSON(filename);
	
	string file_key = stringEncWithAES(filename);
	
	Json::Reader reader;
	reader.parse("{}", file_sync[file_key]);
	testF(filename, "");
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
		testF(filename, writer.write(file_sync[file_key]));
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
		testF(filename, writer.write(file_sync[file_key]));
	}
}
void SaveData::setKeyValue(SaveDataFile t_sdf, string _key, double _value, bool diskWrite /*= true*/){			setKeyValue(getSyncKey(t_sdf), _key, _value, diskWrite);		}
void SaveData::setKeyValue(SaveDataFile t_sdf, int i1, string _key, double _value, bool diskWrite /*= true*/){	setKeyValue(getSyncKey(t_sdf, i1), _key, _value, diskWrite);	}

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
	testF(filename, writer.write(file_sync[file_key]));
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
