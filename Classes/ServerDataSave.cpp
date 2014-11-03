// ServerDataSave.cpp
//

#include "ServerDataSave.h"
#define LZZ_INLINE inline
ServerDataSave * ServerDataSave::sharedInstance ()
{
	static ServerDataSave* t_sds = NULL;
	if(t_sds == NULL)
	{
		t_sds = new ServerDataSave();
		t_sds->myInit();
	}
	return t_sds;
}
bool ServerDataSave::getBoolForKey (SaveDataFile f_key, string r_key)
{
	string c_key = myDefault->getSyncKey(f_key)+r_key;
	
	iter_bool = sds_cache_bool.find(c_key);
	if(iter_bool != sds_cache_bool.end())
		return iter_bool->second.getV();
	
	string bool_string = myDefault->getValue(f_key, r_key, "false");
	if(bool_string == "false")
	{
		sds_cache_bool[c_key] = false;
		return false;
	}
	else
	{
		sds_cache_bool[c_key] = true;
		return true;
	}
}
bool ServerDataSave::gbfk (SDS_KEY fr_key)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getBoolForKey(key_set.f_key, key_set.r_key.c_str());
}
bool ServerDataSave::getBoolForKey (SaveDataFile f_key, string t_key, int key_val1)
{
	string c_key = myDefault->getSyncKey(f_key)+CCString::createWithFormat(t_key.c_str(), key_val1)->getCString();
	
	iter_bool = sds_cache_bool.find(c_key);
	if(iter_bool != sds_cache_bool.end())
		return iter_bool->second.getV();
	
	string bool_string = myDefault->getValue(f_key, CCString::createWithFormat(t_key.c_str(), key_val1)->getCString(), "false");
	if(bool_string == "false")
	{
		sds_cache_bool[c_key] = false;
		return false;
	}
	else
	{
		sds_cache_bool[c_key] = true;
		return true;
	}
}
bool ServerDataSave::gbfk (SDS_KEY fr_key, int key_val1)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getBoolForKey(key_set.f_key, key_set.r_key.c_str(), key_val1);
}
bool ServerDataSave::getBoolForKey (SaveDataFile f_key, int i1, string r_key)
{
	string c_key = myDefault->getSyncKey(f_key, i1)+r_key;
	
	iter_bool = sds_cache_bool.find(c_key);
	if(iter_bool != sds_cache_bool.end())
		return iter_bool->second.getV();
	
	string bool_string = myDefault->getValue(f_key, i1, r_key, "false");
	if(bool_string == "false")
	{
		sds_cache_bool[c_key] = false;
		return false;
	}
	else
	{
		sds_cache_bool[c_key] = true;
		return true;
	}
}
bool ServerDataSave::gbfk (int i1, SDS_KEY fr_key)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getBoolForKey(key_set.f_key, i1, key_set.r_key);
}
bool ServerDataSave::getBoolForKey (SaveDataFile f_key, int i1, string t_key, int key_val1)
{
	string c_key = myDefault->getSyncKey(f_key, i1) + CCString::createWithFormat(t_key.c_str(), key_val1)->getCString();
	
	iter_bool = sds_cache_bool.find(c_key);
	if(iter_bool != sds_cache_bool.end())
		return iter_bool->second.getV();
	
	string bool_string = myDefault->getValue(f_key, i1, CCString::createWithFormat(t_key.c_str(), key_val1)->getCString(), "false");
	if(bool_string == "false")
	{
		sds_cache_bool[c_key] = false;
		return false;
	}
	else
	{
		sds_cache_bool[c_key] = true;
		return true;
	}
}
bool ServerDataSave::gbfk (int i1, SDS_KEY fr_key, int key_val1)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getBoolForKey(key_set.f_key, i1, key_set.r_key, key_val1);
}
void ServerDataSave::setBoolForKey (SaveDataFile f_key, string r_key, bool t_b, bool diskWrite)
{
	string bool_string;
	if(t_b)			bool_string = "true";
	else			bool_string = "false";
	myDefault->setKeyValue(f_key, r_key, bool_string, diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key)+r_key;
	sds_cache_bool[c_key] = t_b;
}
void ServerDataSave::sbfk (SDS_KEY fr_key, bool t_b, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setBoolForKey(key_set.f_key, key_set.r_key, t_b, diskWrite);
}
void ServerDataSave::setBoolForKey (SaveDataFile f_key, string r_key, int key_val1, bool t_b, bool diskWrite)
{
	string bool_string;
	if(t_b)			bool_string = "true";
	else			bool_string = "false";
	myDefault->setKeyValue(f_key, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), bool_string, diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key)+CCString::createWithFormat(r_key.c_str(), key_val1)->getCString();
	sds_cache_bool[c_key] = t_b;
}
void ServerDataSave::sbfk (SDS_KEY fr_key, int key_val1, bool t_b, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setBoolForKey(key_set.f_key, key_set.r_key, key_val1, t_b, diskWrite);
}
void ServerDataSave::setBoolForKey (SaveDataFile f_key, int i1, string r_key, bool t_b, bool diskWrite)
{
	string bool_string;
	if(t_b)			bool_string = "true";
	else			bool_string = "false";
	myDefault->setKeyValue(f_key, i1, r_key, bool_string, diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key, i1)+r_key;
	sds_cache_bool[c_key] = t_b;
}
void ServerDataSave::sbfk (int i1, SDS_KEY fr_key, bool t_b, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setBoolForKey(key_set.f_key, i1, key_set.r_key, t_b, diskWrite);
}
void ServerDataSave::setBoolForKey (SaveDataFile f_key, int i1, string r_key, int key_val1, bool t_b, bool diskWrite)
{
	string bool_string;
	if(t_b)			bool_string = "true";
	else			bool_string = "false";
	myDefault->setKeyValue(f_key, i1, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), bool_string, diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key, i1) + CCString::createWithFormat(r_key.c_str(), key_val1)->getCString();
	sds_cache_bool[c_key] = t_b;
}
void ServerDataSave::sbfk (int i1, SDS_KEY fr_key, int key_val1, bool t_b, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setBoolForKey(key_set.f_key, i1, key_set.r_key, key_val1, t_b, diskWrite);
}
int ServerDataSave::getIntegerForKey (SaveDataFile f_key, string r_key)
{
	string c_key = myDefault->getSyncKey(f_key) + r_key;
	
	iter_int = sds_cache_int.find(c_key);
	if(iter_int != sds_cache_int.end())
		return iter_int->second.getV();
	
	int return_value = myDefault->getValue(f_key, r_key, 0);
	sds_cache_int[c_key] = return_value;
	return return_value;
}
int ServerDataSave::gifk (SDS_KEY fr_key)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getIntegerForKey(key_set.f_key, key_set.r_key);
}
int ServerDataSave::getIntegerForKey (SaveDataFile f_key, string r_key, int key_val1)
{
	string c_key = myDefault->getSyncKey(f_key) + CCString::createWithFormat(r_key.c_str(), key_val1)->getCString();
	
	iter_int = sds_cache_int.find(c_key);
	if(iter_int != sds_cache_int.end())
		return iter_int->second.getV();
	
	int return_value = myDefault->getValue(f_key, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), 0);
	sds_cache_int[c_key] = return_value;
	return return_value;
}
int ServerDataSave::gifk (SDS_KEY fr_key, int key_val1)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getIntegerForKey(key_set.f_key, key_set.r_key, key_val1);
}
int ServerDataSave::getIntegerForKey (SaveDataFile f_key, string r_key, int key_val1, int key_val2)
{
	string c_key = myDefault->getSyncKey(f_key) + CCString::createWithFormat(r_key.c_str(), key_val1, key_val2)->getCString();
	
	iter_int = sds_cache_int.find(c_key);
	if(iter_int != sds_cache_int.end())
		return iter_int->second.getV();
	
	int return_value = myDefault->getValue(f_key, CCString::createWithFormat(r_key.c_str(), key_val1, key_val2)->getCString(), 0);
	sds_cache_int[c_key] = return_value;
	return return_value;
}
int ServerDataSave::gifk (SDS_KEY fr_key, int key_val1, int key_val2)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getIntegerForKey(key_set.f_key, key_set.r_key, key_val1, key_val2);
}
int ServerDataSave::getIntegerForKey (SaveDataFile f_key, int i1, string r_key)
{
	string c_key = myDefault->getSyncKey(f_key, i1) + r_key;
	
	iter_int = sds_cache_int.find(c_key);
	if(iter_int != sds_cache_int.end())
		return iter_int->second.getV();
	
	int return_value = myDefault->getValue(f_key, i1, r_key, 0);
	sds_cache_int[c_key] = return_value;
	return return_value;
}
int ServerDataSave::gifk (int i1, SDS_KEY fr_key)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getIntegerForKey(key_set.f_key, i1, key_set.r_key);
}
int ServerDataSave::getIntegerForKey (SaveDataFile f_key, int i1, string r_key, int key_val1)
{
	string c_key = myDefault->getSyncKey(f_key, i1) + CCString::createWithFormat(r_key.c_str(), key_val1)->getCString();
	
	iter_int = sds_cache_int.find(c_key);
	if(iter_int != sds_cache_int.end())
		return iter_int->second.getV();
	
	int return_value = myDefault->getValue(f_key, i1, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), 0);
	sds_cache_int[c_key] = return_value;
	return return_value;
}
int ServerDataSave::gifk (int i1, SDS_KEY fr_key, int key_val1)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getIntegerForKey(key_set.f_key, i1, key_set.r_key, key_val1);
}
void ServerDataSave::setIntegerForKey (SaveDataFile f_key, string r_key, int val1, bool diskWrite)
{
	myDefault->setKeyValue(f_key, r_key, val1, diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key) + r_key;
	sds_cache_int[c_key] = val1;
}
void ServerDataSave::sifk (SDS_KEY fr_key, int val1, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setIntegerForKey(key_set.f_key, key_set.r_key, val1, diskWrite);
}
void ServerDataSave::setIntegerForKey (SaveDataFile f_key, string r_key, int key_val1, int val1, bool diskWrite)
{
	myDefault->setKeyValue(f_key, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), val1, diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key) + CCString::createWithFormat(r_key.c_str(), key_val1)->getCString();
	sds_cache_int[c_key] = val1;
}
void ServerDataSave::sifk (SDS_KEY fr_key, int key_val1, int val1, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setIntegerForKey(key_set.f_key, key_set.r_key, key_val1, val1, diskWrite);
}
void ServerDataSave::setIntegerForKey (SaveDataFile f_key, string r_key, int key_val1, int key_val2, int val1, bool diskWrite)
{
	myDefault->setKeyValue(f_key, CCString::createWithFormat(r_key.c_str(), key_val1, key_val2)->getCString(), val1, diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key) + CCString::createWithFormat(r_key.c_str(), key_val1, key_val2)->getCString();
	sds_cache_int[c_key] = val1;
}
void ServerDataSave::sifk (SDS_KEY fr_key, int key_val1, int key_val2, int val1, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setIntegerForKey(key_set.f_key, key_set.r_key, key_val1, key_val2, val1, diskWrite);
}
void ServerDataSave::setIntegerForKey (SaveDataFile f_key, int i1, string r_key, int val1, bool diskWrite)
{
	myDefault->setKeyValue(f_key, i1, r_key, val1, diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key, i1) + r_key;
	sds_cache_int[c_key] = val1;
}
void ServerDataSave::sifk (int i1, SDS_KEY fr_key, int val1, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setIntegerForKey(key_set.f_key, i1, key_set.r_key, val1, diskWrite);
}
void ServerDataSave::setIntegerForKey (SaveDataFile f_key, int i1, string r_key, int key_val1, int val1, bool diskWrite)
{
	myDefault->setKeyValue(f_key, i1, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), val1, diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key, i1) + CCString::createWithFormat(r_key.c_str(), key_val1)->getCString();
	sds_cache_int[c_key] = val1;
}
void ServerDataSave::sifk (int i1, SDS_KEY fr_key, int key_val1, int val1, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setIntegerForKey(key_set.f_key, i1, key_set.r_key, key_val1, val1, diskWrite);
}
double ServerDataSave::getDoubleForKey (SaveDataFile f_key, string r_key)
{
	string c_key = myDefault->getSyncKey(f_key)+r_key;
	
	iter_double = sds_cache_double.find(c_key);
	if(iter_double != sds_cache_double.end())
		return iter_double->second.getV();
	
	double return_value = myDefault->getValue(f_key, r_key, 0.0);
	sds_cache_double[c_key] = return_value;
	return return_value;
}
double ServerDataSave::gdfk (SDS_KEY fr_key)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getDoubleForKey(key_set.f_key, key_set.r_key);
}
double ServerDataSave::getDoubleForKey (SaveDataFile f_key, string r_key, int key_val1)
{
	string c_key = myDefault->getSyncKey(f_key) + CCString::createWithFormat(r_key.c_str(), key_val1)->getCString();
	
	iter_double = sds_cache_double.find(c_key);
	if(iter_double != sds_cache_double.end())
		return iter_double->second.getV();
	
	double return_value = myDefault->getValue(f_key, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), 0.0);
	sds_cache_double[c_key] = return_value;
	return return_value;
}
double ServerDataSave::gdfk (SDS_KEY fr_key, int key_val1)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getDoubleForKey(key_set.f_key, key_set.r_key, key_val1);
}
double ServerDataSave::getDoubleForKey (SaveDataFile f_key, string r_key, int key_val1, int key_val2)
{
	string c_key = myDefault->getSyncKey(f_key) + CCString::createWithFormat(r_key.c_str(), key_val1, key_val2)->getCString();
	
	iter_double = sds_cache_double.find(c_key);
	if(iter_double != sds_cache_double.end())
		return iter_double->second.getV();
	
	double return_value = myDefault->getValue(f_key, CCString::createWithFormat(r_key.c_str(), key_val1, key_val2)->getCString(), 0.0);
	sds_cache_double[c_key] = return_value;
	return return_value;
}
double ServerDataSave::gdfk (SDS_KEY fr_key, int key_val1, int key_val2)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getDoubleForKey(key_set.f_key, key_set.r_key, key_val1, key_val2);
}
double ServerDataSave::getDoubleForKey (SaveDataFile f_key, int i1, string r_key)
{
	string c_key = myDefault->getSyncKey(f_key, i1) + r_key;
	
	iter_double = sds_cache_double.find(c_key);
	if(iter_double != sds_cache_double.end())
		return iter_double->second.getV();
	
	double return_value = myDefault->getValue(f_key, i1, r_key, 0.0);
	sds_cache_double[c_key] = return_value;
	return return_value;
}
double ServerDataSave::gdfk (int i1, SDS_KEY fr_key)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getDoubleForKey(key_set.f_key, i1, key_set.r_key);
}
double ServerDataSave::getDoubleForKey (SaveDataFile f_key, int i1, string r_key, int key_val1)
{
	string c_key = myDefault->getSyncKey(f_key, i1) + CCString::createWithFormat(r_key.c_str(), key_val1)->getCString();
	
	iter_double = sds_cache_double.find(c_key);
	if(iter_double != sds_cache_double.end())
		return iter_double->second.getV();
	
	double return_value = myDefault->getValue(f_key, i1, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), 0.0);
	sds_cache_double[c_key] = return_value;
	return return_value;
}
double ServerDataSave::gdfk (int i1, SDS_KEY fr_key, int key_val1)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getDoubleForKey(key_set.f_key, i1, key_set.r_key, key_val1);
}
void ServerDataSave::setDoubleForKey (SaveDataFile f_key, string r_key, double val1, bool diskWrite)
{
	myDefault->setKeyValue(f_key, r_key, val1, diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key)+r_key;
	sds_cache_double[c_key] = val1;
}
void ServerDataSave::sdfk (SDS_KEY fr_key, double val1, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setDoubleForKey(key_set.f_key, key_set.r_key, val1, diskWrite);
}
void ServerDataSave::setDoubleForKey (SaveDataFile f_key, string r_key, int key_val1, double val1, bool diskWrite)
{
	myDefault->setKeyValue(f_key, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), val1, diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key) + CCString::createWithFormat(r_key.c_str(), key_val1)->getCString();
	sds_cache_double[c_key] = val1;
}
void ServerDataSave::sdfk (SDS_KEY fr_key, int key_val1, double val1, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setDoubleForKey(key_set.f_key, key_set.r_key, key_val1, val1, diskWrite);
}
void ServerDataSave::setDoubleForKey (SaveDataFile f_key, string r_key, int key_val1, int key_val2, double val1, bool diskWrite)
{
	myDefault->setKeyValue(f_key, CCString::createWithFormat(r_key.c_str(), key_val1, key_val2)->getCString(), val1, diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key) + CCString::createWithFormat(r_key.c_str(), key_val1, key_val2)->getCString();
	sds_cache_double[c_key] = val1;
}
void ServerDataSave::sdfk (SDS_KEY fr_key, int key_val1, int key_val2, double val1, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setDoubleForKey(key_set.f_key, key_set.r_key, key_val1, key_val2, val1, diskWrite);
}
void ServerDataSave::setDoubleForKey (SaveDataFile f_key, int i1, string r_key, double val1, bool diskWrite)
{
	myDefault->setKeyValue(f_key, i1, r_key, val1, diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key, i1) + r_key;
	sds_cache_double[c_key] = val1;
}
void ServerDataSave::sdfk (int i1, SDS_KEY fr_key, double val1, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setDoubleForKey(key_set.f_key, i1, key_set.r_key, val1, diskWrite);
}
void ServerDataSave::setDoubleForKey (SaveDataFile f_key, int i1, string r_key, int key_val1, double val1, bool diskWrite)
{
	myDefault->setKeyValue(f_key, i1, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), val1, diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key, i1) + CCString::createWithFormat(r_key.c_str(), key_val1)->getCString();
	sds_cache_double[c_key] = val1;
}
void ServerDataSave::sdfk (int i1, SDS_KEY fr_key, int key_val1, double val1, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setDoubleForKey(key_set.f_key, i1, key_set.r_key, key_val1, val1, diskWrite);
}
string ServerDataSave::getStringForKey (SaveDataFile f_key, string r_key)
{
	string c_key = myDefault->getSyncKey(f_key) + r_key;
	
	iter_string = sds_cache_string.find(c_key);
	if(iter_string != sds_cache_string.end())
		return iter_string->second.getV();
	
	string return_value = myDefault->getValue(f_key, r_key, "");
	sds_cache_string[c_key] = return_value;
	return return_value;
}
string ServerDataSave::gsfk (SDS_KEY fr_key)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getStringForKey(key_set.f_key, key_set.r_key);
}
string ServerDataSave::getStringForKey (SaveDataFile f_key, string r_key, int key_val1)
{
	string c_key = myDefault->getSyncKey(f_key) + CCString::createWithFormat(r_key.c_str(), key_val1)->getCString();
	
	iter_string = sds_cache_string.find(c_key);
	if(iter_string != sds_cache_string.end())
		return iter_string->second.getV();
	
	string return_value = myDefault->getValue(f_key, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), "");
	sds_cache_string[c_key] = return_value;
	return return_value;
}
string ServerDataSave::gsfk (SDS_KEY fr_key, int key_val1)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getStringForKey(key_set.f_key, key_set.r_key, key_val1);
}
string ServerDataSave::getStringForKey (SaveDataFile f_key, string r_key, int key_val1, int key_val2)
{
	string c_key = myDefault->getSyncKey(f_key) + CCString::createWithFormat(r_key.c_str(), key_val1, key_val2)->getCString();
	
	iter_string = sds_cache_string.find(c_key);
	if(iter_string != sds_cache_string.end())
		return iter_string->second.getV();
	
	string return_value = myDefault->getValue(f_key, CCString::createWithFormat(r_key.c_str(), key_val1, key_val2)->getCString(), "");
	sds_cache_string[c_key] = return_value;
	return return_value;
}
string ServerDataSave::gsfk (SDS_KEY fr_key, int key_val1, int key_val2)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getStringForKey(key_set.f_key, key_set.r_key, key_val1, key_val2);
}
string ServerDataSave::getStringForKey (SaveDataFile f_key, int i1, string r_key)
{
	string c_key = myDefault->getSyncKey(f_key, i1) + r_key;
	
	iter_string = sds_cache_string.find(c_key);
	if(iter_string != sds_cache_string.end())
		return iter_string->second.getV();
	
	string return_value = myDefault->getValue(f_key, i1, r_key, "");
	sds_cache_string[c_key] = return_value;
	return return_value;
}
string ServerDataSave::gsfk (int i1, SDS_KEY fr_key)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getStringForKey(key_set.f_key, i1, key_set.r_key);
}
string ServerDataSave::getStringForKey (SaveDataFile f_key, int i1, string r_key, int key_val1)
{
	string c_key = myDefault->getSyncKey(f_key, i1) + CCString::createWithFormat(r_key.c_str(), key_val1)->getCString();
	
	iter_string = sds_cache_string.find(c_key);
	if(iter_string != sds_cache_string.end())
		return iter_string->second.getV();
	
	string return_value = myDefault->getValue(f_key, i1, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), "");
	sds_cache_string[c_key] = return_value;
	return return_value;
}
string ServerDataSave::gsfk (int i1, SDS_KEY fr_key, int key_val1)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getStringForKey(key_set.f_key, i1, key_set.r_key, key_val1);
}
void ServerDataSave::setStringForKey (SaveDataFile f_key, string r_key, string val1, bool diskWrite)
{
	myDefault->setKeyValue(f_key, r_key, val1.c_str(), diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key) + r_key;
	sds_cache_string[c_key] = val1;
}
void ServerDataSave::ssfk (SDS_KEY fr_key, string val1, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setStringForKey(key_set.f_key, key_set.r_key, val1.c_str(), diskWrite);
}
void ServerDataSave::setStringForKey (SaveDataFile f_key, string r_key, int key_val1, string val1, bool diskWrite)
{
	myDefault->setKeyValue(f_key, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), val1.c_str(), diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key) + CCString::createWithFormat(r_key.c_str(), key_val1)->getCString();
	sds_cache_string[c_key] = val1;
}
void ServerDataSave::ssfk (SDS_KEY fr_key, int key_val1, string val1, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setStringForKey(key_set.f_key, key_set.r_key, key_val1, val1.c_str(), diskWrite);
}
void ServerDataSave::setStringForKey (SaveDataFile f_key, string r_key, int key_val1, int key_val2, string val1, bool diskWrite)
{
	myDefault->setKeyValue(f_key, CCString::createWithFormat(r_key.c_str(), key_val1, key_val2)->getCString(), val1.c_str(), diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key) + CCString::createWithFormat(r_key.c_str(), key_val1, key_val2)->getCString();
	sds_cache_string[c_key] = val1;
}
void ServerDataSave::ssfk (SDS_KEY fr_key, int key_val1, int key_val2, string val1, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setStringForKey(key_set.f_key, key_set.r_key, key_val1, key_val2, val1.c_str(), diskWrite);
}
void ServerDataSave::setStringForKey (SaveDataFile f_key, int i1, string r_key, string val1, bool diskWrite)
{
	myDefault->setKeyValue(f_key, i1, r_key, val1.c_str(), diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key, i1) + r_key;
	sds_cache_string[c_key] = val1;
}
void ServerDataSave::ssfk (int i1, SDS_KEY fr_key, string val1, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setStringForKey(key_set.f_key, i1, key_set.r_key, val1.c_str(), diskWrite);
}
void ServerDataSave::setStringForKey (SaveDataFile f_key, int i1, string r_key, int key_val1, string val1, bool diskWrite)
{
	myDefault->setKeyValue(f_key, i1, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), val1.c_str(), diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key, i1) + CCString::createWithFormat(r_key.c_str(), key_val1)->getCString();
	sds_cache_string[c_key] = val1;
}
void ServerDataSave::ssfk (int i1, SDS_KEY fr_key, int key_val1, string val1, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setStringForKey(key_set.f_key, i1, key_set.r_key, key_val1, val1.c_str(), diskWrite);
}
SDS_SET ServerDataSave::getKeySet (SDS_KEY t_key)
{
	SDS_SET rv;
	rv.f_key = getSDF(t_key);
	rv.r_key = getRKey(t_key).c_str();
	return rv;
}
string ServerDataSave::getRKey (SDS_KEY t_key)
{
	string rv;
	
	if(t_key == kSDS_PZ_version_i)						rv = "version";
	else if(t_key == kSDS_PZ_title_s)					rv = "title";
	else if(t_key == kSDS_PZ_condition_s)				rv = "condition";
	else if(t_key == kSDS_PZ_ticket_i)					rv = "ticket";
	else if(t_key == kSDS_PZ_point_i)					rv = "point";
	else if(t_key == kSDS_PZ_center_s)					rv = "center";
	else if(t_key == kSDS_PZ_original_s)				rv = "original";
	else if(t_key == kSDS_PZ_face_s)					rv = "face";
	else if(t_key == kSDS_PZ_map_s)						rv = "map";
	else if(t_key == kSDS_PZ_bottom_s)					rv = "bottom";
	else if(t_key == kSDS_PZ_top_s)						rv = "top";
	else if(t_key == kSDS_PZ_left_s)					rv = "left";
	else if(t_key == kSDS_PZ_right_s)					rv = "right";
	else if(t_key == kSDS_PZ_startStage_i)				rv = "startStage";
	else if(t_key == kSDS_PZ_stageCount_i)				rv = "stageCount";
	else if(t_key == kSDS_PZ_stage_int1_level_i)		rv = "stage%d_level";
	else if(t_key == kSDS_PZ_stage_int1_pieceType_s)	rv = "stage%d_pieceType";
	else if(t_key == kSDS_PZ_stage_int1_pieceNo_i)		rv = "stage%d_pieceNo";
	else if(t_key == kSDS_PZ_stage_int1_x_d)			rv = "stage%d_x";
	else if(t_key == kSDS_PZ_stage_int1_y_d)			rv = "stage%d_y";
	else if(t_key == kSDS_PZ_stage_int1_piece_s)		rv = "stage%d_piece";
	else if(t_key == kSDS_PZ_stage_int1_thumbnail_s)	rv = "stage%d_thumbnail";
	else if(t_key == kSDS_PZ_stage_int1_condition_stage_i)	rv = "stage%d_condition_stage";
	else if(t_key == kSDS_PZ_stage_int1_condition_gold_i)	rv = "stage%d_condition_gold";
	else if(t_key == kSDS_PZ_startWarp_x_d)				rv = "swarp_x";
	else if(t_key == kSDS_PZ_startWarp_y_d)				rv = "swarp_y";
	else if(t_key == kSDS_PZ_lastWarp_x_d)				rv = "lwarp_x";
	else if(t_key == kSDS_PZ_lastWarp_y_d)				rv = "lwarp_y";
	else if(t_key == kSDS_PZ_color_r_d)					rv = "color_r";
	else if(t_key == kSDS_PZ_color_g_d)					rv = "color_g";
	else if(t_key == kSDS_PZ_color_b_d)					rv = "color_b";
	else if(t_key == kSDS_PZ_isEvent_b)					rv = "isEvent";
	else if(t_key == kSDS_PZ_clearCard_i)				rv = "clearCard";
	else if(t_key == kSDS_PZ_perfectCard_i)				rv = "perfectCard";
	
	else if(t_key == kSDS_GI_puzzleListVersion_i)		rv = "puzzleListVersion";
	else if(t_key == kSDS_GI_puzzleListCount_i)			rv = "puzzleListCount";
	else if(t_key == kSDS_GI_puzzleList_int1_no_i)		rv = "puzzleList%d_no";
	else if(t_key == kSDS_GI_puzzleList_int1_title_s)	rv = "puzzleList%d_title";
	else if(t_key == kSDS_GI_puzzleList_int1_version_i)	rv = "puzzleList%d_version";
	else if(t_key == kSDS_GI_puzzleList_int1_thumbnail_s)			rv = "puzzleList%d_thumbnail";
	
	else if(t_key == kSDS_GI_eventListVersion_i)		rv = "eventListVersion";
	else if(t_key == kSDS_GI_eventListCount_i)			rv = "eventListCount";
	else if(t_key == kSDS_GI_eventList_int1_no_i)		rv = "eventList%d_no";
	else if(t_key == kSDS_GI_eventList_int1_title_s)	rv = "eventList%d_title";
	else if(t_key == kSDS_GI_eventList_int1_version_i)	rv = "eventList%d_version";
	else if(t_key == kSDS_GI_eventList_int1_thumbnail_s)			rv = "eventList%d_thumbnail";
	
	else if(t_key == kSDS_GI_characterVersion_i)		rv = "characterVersion";
	else if(t_key == kSDS_GI_characterCount_i)			rv = "characterCount";
	else if(t_key == kSDS_GI_characterInfo_int1_no_i)		rv = "ci%d_no";
	else if(t_key == kSDS_GI_characterInfo_int1_name_s)		rv = "ci%d_name";
	else if(t_key == kSDS_GI_characterInfo_int1_purchaseInfo_type_s)	rv = "ci%d_pi_type";
	else if(t_key == kSDS_GI_characterInfo_int1_purchaseInfo_value_i)	rv = "ci%d_pi_value";
	else if(t_key == kSDS_GI_characterInfo_int1_maxLevel_i)				rv = "ci%d_mxlv";
	else if(t_key == kSDS_GI_characterInfo_int1_statInfo_int2_gold_d)			rv = "ci%d_si%d_gold";
	else if(t_key == kSDS_GI_characterInfo_int1_statInfo_int2_percent_d)		rv = "ci%d_si%d_percent";
	else if(t_key == kSDS_GI_characterInfo_int1_statInfo_int2_speed_d)			rv = "ci%d_si%d_speed";
	else if(t_key == kSDS_GI_characterInfo_int1_statInfo_int2_life_i)			rv = "ci%d_si%d_life";
	else if(t_key == kSDS_GI_characterInfo_int1_statInfo_int2_lineColor_i)		rv = "ci%d_si%d_lineColor";
	else if(t_key == kSDS_GI_characterInfo_int1_statInfo_int2_rewindSpd_d)		rv = "ci%d_si%d_rewindSpd";
	else if(t_key == kSDS_GI_characterInfo_int1_statInfo_int2_monsterWave_d)	rv = "ci%d_si%d_monsterWave";
	else if(t_key == kSDS_GI_characterInfo_int1_statInfo_int2_missileWave_d)	rv = "ci%d_si%d_missileWave";
	else if(t_key == kSDS_GI_characterInfo_int1_statInfo_int2_magnetic_d)		rv = "ci%d_si%d_magnetic";
	else if(t_key == kSDS_GI_characterInfo_int1_statInfo_int2_power_d)			rv = "ci%d_si%d_power";
	else if(t_key == kSDS_GI_characterInfo_int1_statInfo_int2_score_d)			rv = "ci%d_si%d_score";
	else if(t_key == kSDS_GI_characterInfo_int1_patternInfo_int2_s)			rv = "ci%d_pI%d";
	else if(t_key == kSDS_GI_characterInfo_int1_missionInfo_int2_s)			rv = "ci%d_mI%d";
	else if(t_key == kSDS_GI_characterInfo_int1_missileInfo_int2_s)			rv = "ci%d_mslI%d";
	else if(t_key == kSDS_GI_characterInfo_int1_resourceInfo_ccbiID_s)	rv = "ci%d_ri_ccbiID";
	else if(t_key == kSDS_GI_characterInfo_int1_resourceInfo_ccbi_s)	rv = "ci%d_ri_ccbi";
	else if(t_key == kSDS_GI_characterInfo_int1_resourceInfo_imageID_s)	rv = "ci%d_ri_imageID";
	else if(t_key == kSDS_GI_characterInfo_int1_resourceInfo_plist_s)	rv = "ci%d_ri_plist";
	else if(t_key == kSDS_GI_characterInfo_int1_resourceInfo_pvrccz_s)	rv = "ci%d_ri_pvrccz";
	else if(t_key == kSDS_GI_characterInfo_int1_resourceInfo_size_i)	rv = "ci%d_ri_size";
	else if(t_key == kSDS_GI_characterInfo_int1_comment_int2_s)			rv = "ci%d_comment%d";
	else if(t_key == kSDS_GI_characterInfo_int1_scriptInfo_s)			rv = "ci%d_scrIf";
	
	else if(t_key == kSDS_GI_monsterVersion_i)			rv = "monsterVersion";
	else if(t_key == kSDS_GI_monsterCount_i)			rv = "monsterCount";
	else if(t_key == kSDS_GI_monsterInfo_int1_no_i)			rv = "mi%d_no";
	else if(t_key == kSDS_GI_monsterInfo_int1_name_s)		rv = "mi%d_name";
	else if(t_key == kSDS_GI_monsterInfo_int1_isBoss_b)		rv = "mi%d_isBoss";
	else if(t_key == kSDS_GI_monsterInfo_int1_resourceInfo_ccbiID_s)	rv = "mi%d_ri_ccbiID";
	else if(t_key == kSDS_GI_monsterInfo_int1_resourceInfo_ccbi_s)		rv = "mi%d_ri_ccbi";
	else if(t_key == kSDS_GI_monsterInfo_int1_resourceInfo_imageID_s)	rv = "mi%d_ri_imageID";
	else if(t_key == kSDS_GI_monsterInfo_int1_resourceInfo_plist_s)		rv = "mi%d_ri_plist";
	else if(t_key == kSDS_GI_monsterInfo_int1_resourceInfo_pvrccz_s)	rv = "mi%d_ri_pvrccz";
	else if(t_key == kSDS_GI_monsterInfo_int1_resourceInfo_size_i)		rv = "mi%d_ri_size";
	else if(t_key == kSDS_GI_monsterInfo_int1_script_start_s)			rv = "mi%d_ss";
	else if(t_key == kSDS_GI_monsterInfo_int1_script_clear_s)			rv = "mi%d_sc";
	else if(t_key == kSDS_GI_monsterInfo_int1_script_fail_s)			rv = "mi%d_sf";
	
	else if(t_key == kSDS_GI_shopVersion_i)				rv = "shopVersion";
//	else if(t_key == kSDS_GI_shopRuby_int1_count_i)				rv = "sr%dcount";
	else if(t_key == kSDS_GI_shopRuby_int1_countName_s)			rv = "sr%dcntName";
//	else if(t_key == kSDS_GI_shopRuby_int1_price_i)				rv = "sr%dprice";
//	else if(t_key == kSDS_GI_shopRuby_int1_priceType_s)			rv = "sr%dprcType";
	else if(t_key == kSDS_GI_shopRuby_int1_priceName_s)			rv = "sr%dprcName";
	else if(t_key == kSDS_GI_shopRuby_int1_sale_s)				rv = "sr%dsale";
	else if(t_key == kSDS_GI_shopRuby_int1_pID_s)				rv = "sr%dpID";
	else if(t_key == kSDS_GI_shopRuby_int1_exchangeID_s)		rv = "sr%dxcID";
//	else if(t_key == kSDS_GI_shopGold_int1_count_i)				rv = "sg%dcount";
	else if(t_key == kSDS_GI_shopGold_int1_countName_s)			rv = "sg%dcntName";
//	else if(t_key == kSDS_GI_shopGold_int1_price_i)				rv = "sg%dprice";
//	else if(t_key == kSDS_GI_shopGold_int1_priceType_s)			rv = "sg%dprcType";
	else if(t_key == kSDS_GI_shopGold_int1_priceName_s)			rv = "sg%dprcName";
	else if(t_key == kSDS_GI_shopGold_int1_sale_s)				rv = "sg%dsale";
	else if(t_key == kSDS_GI_shopGold_int1_exchangeID_s)		rv = "sg%dxcID";
//	else if(t_key == kSDS_GI_shopCoin_int1_count_i)				rv = "sc%dcount";
	else if(t_key == kSDS_GI_shopCoin_int1_countName_s)			rv = "sc%dcntName";
//	else if(t_key == kSDS_GI_shopCoin_int1_price_i)				rv = "sc%dprice";
//	else if(t_key == kSDS_GI_shopCoin_int1_priceType_s)			rv = "sc%dprcType";
	else if(t_key == kSDS_GI_shopCoin_int1_priceName_s)			rv = "sc%dprcName";
	else if(t_key == kSDS_GI_shopCoin_int1_sale_s)				rv = "sc%dsale";
	else if(t_key == kSDS_GI_shopCoin_int1_exchangeID_s)		rv = "sc%dxcID";
//	else if(t_key == kSDS_GI_shopEventRuby_int1_count_i)		rv = "esr%dcount";
	else if(t_key == kSDS_GI_shopEventRuby_int1_countName_s)	rv = "esr%dcntName";
//	else if(t_key == kSDS_GI_shopEventRuby_int1_price_i)		rv = "esr%dprice";
//	else if(t_key == kSDS_GI_shopEventRuby_int1_priceType_s)	rv = "esr%dprcType";
	else if(t_key == kSDS_GI_shopEventRuby_int1_priceName_s)	rv = "esr%dprcName";
	else if(t_key == kSDS_GI_shopEventRuby_int1_sale_s)			rv = "esr%dsale";
	else if(t_key == kSDS_GI_shopEventRuby_int1_pID_s)			rv = "esr%dpID";
	else if(t_key == kSDS_GI_shopEventRuby_int1_exchangeID_s)		rv = "esr%dxcID";
//	else if(t_key == kSDS_GI_shopPurchaseGuide_int1_count_i)		rv = "sp%dcount";
	else if(t_key == kSDS_GI_shopPurchaseGuide_int1_countName_s)	rv = "sp%dcntName";
//	else if(t_key == kSDS_GI_shopPurchaseGuide_int1_price_i)		rv = "sp%dprice";
//	else if(t_key == kSDS_GI_shopPurchaseGuide_int1_priceType_s)	rv = "sp%dprcType";
	else if(t_key == kSDS_GI_shopPurchaseGuide_int1_priceName_s)	rv = "sp%dprcName";
	else if(t_key == kSDS_GI_shopPurchaseGuide_int1_sale_s)			rv = "sp%dsale";
	else if(t_key == kSDS_GI_shopPurchaseGuide_int1_data_s)			rv = "sp%ddata";
	else if(t_key == kSDS_GI_shopPurchaseGuide_int1_pID_s)			rv = "sp%dpID";
	else if(t_key == kSDS_GI_shopPurchaseGuide_int1_exchangeID_s)	rv = "sp%dxcID";
	else if(t_key == kSDS_GI_shopItem_int1_countName_s)			rv = "si%dcntName";
//	else if(t_key == kSDS_GI_shopItem_int1_price_i)				rv = "si%dprice";
//	else if(t_key == kSDS_GI_shopItem_int1_priceType_s)			rv = "si%dprcType";
	else if(t_key == kSDS_GI_shopItem_int1_priceName_s)			rv = "si%dprcName";
	else if(t_key == kSDS_GI_shopItem_int1_sale_s)				rv = "si%dsale";
	else if(t_key == kSDS_GI_shopItem_int1_exchangeID_s)		rv = "si%dxcID";
	else if(t_key == kSDS_GI_shopP1_int1_countName_s)			rv = "sp1%dcntName";
	else if(t_key == kSDS_GI_shopP1_int1_priceName_s)			rv = "sp1%dprcName";
	else if(t_key == kSDS_GI_shopP1_int1_sale_s)				rv = "sp1%dsale";
	else if(t_key == kSDS_GI_shopP1_int1_exchangeID_s)			rv = "sp1%dxcID";
	else if(t_key == kSDS_GI_shop_isEvent_b)					rv = "sIsEv";
	else if(t_key == kSDS_GI_shopStartPack_no_i)				rv = "sspNo";
	else if(t_key == kSDS_GI_shopStartPack_pID_s)				rv = "sspPID";
	else if(t_key == kSDS_GI_shopStartPack_exchangeID_s)		rv = "sspXcID";
	else if(t_key == kSDS_GI_shopStartPack_expireSec_i)			rv = "sspExpireSec";
	else if(t_key == kSDS_GI_shopStartPack_img_s)				rv = "sspImg";
	else if(t_key == kSDS_GI_shopEventPack_isHave_b)			rv = "sepIsHave";
	else if(t_key == kSDS_GI_shopEventPack_no_i)				rv = "sepNo";
	else if(t_key == kSDS_GI_shopEventPack_pID_s)				rv = "sepPID";
	else if(t_key == kSDS_GI_shopEventPack_exchangeID_s)		rv = "sepXcID";
	else if(t_key == kSDS_GI_shopEventPack_startDate_s)			rv = "sepStartDate";
	else if(t_key == kSDS_GI_shopEventPack_endDate_s)			rv = "sepEndDate";
	else if(t_key == kSDS_GI_shopEventPack_startTime_i)			rv = "sepStartTime";
	else if(t_key == kSDS_GI_shopEventPack_endTime_i)			rv = "sepEndTime";
	else if(t_key == kSDS_GI_shopEventPack_isJustOne_b)			rv = "sepIsJustOne";
	else if(t_key == kSDS_GI_shopEventPack_img_s)				rv = "sepImg";
	else if(t_key == kSDS_GI_serial_int1_cardNumber_i)			rv = "srl%dcd";
	else if(t_key == kSDS_GI_hellMode_version_i)				rv = "hlVersion";
	else if(t_key == kSDS_GI_hellMode_listCount_i)				rv = "hlListCnt";
	else if(t_key == kSDS_GI_hellMode_int1_openPieceNo_i)		rv = "hl%dopn";
	else if(t_key == kSDS_GI_hellMode_int1_cellImgInfo_s)		rv = "hl%dcellImg";
	else if(t_key == kSDS_GI_hellMode_int1_pieceNo_i)			rv = "hl%dpcNo";
	else if(t_key == kSDS_GI_hellMode_int1_title_s)				rv = "hl%dtitle";
	else if(t_key == kSDS_GI_hellMode_int1_content_s)			rv = "hl%dcontent";
	else if(t_key == kSDS_GI_hellMode_int1_characterNo_i)		rv = "hl%dcharacterNo";
	else if(t_key == kSDS_GI_realTimeMessageVersion_i)			rv = "rtmv";
	else if(t_key == kSDS_GI_card_version_i)					rv = "cardVersion";
	
	else if(t_key == kSDS_SI_version_i)					rv = "version";
	else if(t_key == kSDS_SI_puzzle_i)					rv = "puzzle";
	else if(t_key == kSDS_SI_playtime_i)				rv = "playtime";
	else if(t_key == kSDS_SI_scoreRate_d)				rv = "scoreRate";
	else if(t_key == kSDS_SI_scale_d)					rv = "scale";
	else if(t_key == kSDS_SI_missionType_i)				rv = "mission_type";
	else if(t_key == kSDS_SI_missionOptionEnergy_i)		rv = "mission_option_energy";
	else if(t_key == kSDS_SI_missionOptionCount_i)		rv = "mission_option_count";
	else if(t_key == kSDS_SI_missionOptionPercent_i)	rv = "mission_option_percent";
	else if(t_key == kSDS_SI_missionOptionSec_i)		rv = "mission_option_sec";
	else if(t_key == kSDS_SI_shopItemsCnt_i)			rv = "shopItems_cnt";
	else if(t_key == kSDS_SI_shopItems_int1_currency_s)	rv = "shopItems_%d_currency";
	else if(t_key == kSDS_SI_shopItems_int1_type_i)		rv = "shopItems_%d_type";
	else if(t_key == kSDS_SI_shopItems_int1_price_i)	rv = "shopItems_%d_price";
	else if(t_key == kSDS_SI_defItemsCnt_i)				rv = "defItems_cnt";
	else if(t_key == kSDS_SI_defItems_int1_type_i)		rv = "defItems_%d_type";
	else if(t_key == kSDS_SI_itemOptionAttackPower_i)	rv = "itemOption_attack_power";
	else if(t_key == kSDS_SI_itemOptionAddTimeSec_i)	rv = "itemOption_addTime_sec";
	else if(t_key == kSDS_SI_itemOptionFastSec_i)		rv = "itemOption_fast_sec";
	else if(t_key == kSDS_SI_itemOptionSilenceSec_i)	rv = "itemOption_silence_sec";
	else if(t_key == kSDS_SI_itemOptionDoubleItemPercent_i)			rv = "itemOption_doubleItem_percent";
	else if(t_key == kSDS_SI_itemOptionLongTimeSec_i)	rv = "itemOption_longTime_sec";
	else if(t_key == kSDS_SI_itemOptionBaseSpeedUpUnit_i)	rv = "itemOption_baseSpeedUp_unit";
	else if(t_key == kSDS_SI_cardCount_i)				rv = "cardCount";
	else if(t_key == kSDS_SI_level_int1_card_i)			rv = "level%d_card";
	else if(t_key == kSDS_SI_level_i)					rv = "level";
	else if(t_key == kSDS_SI_boss_s)					rv = "boss";
	else if(t_key == kSDS_SI_junior_s)					rv = "junior";
	else if(t_key == kSDS_SI_autoBalanceTry_i)			rv = "%d_autoBalanceTry";
	else if(t_key == kSDS_SI_type_s)					rv = "type";
	
	else if(t_key == kSDS_CI_int1_rank_i)						rv = "%d_grade";
	else if(t_key == kSDS_CI_int1_grade_i)						rv = "%d_rank";
	else if(t_key == kSDS_CI_int1_durability_i)					rv = "%d_durability";
	else if(t_key == kSDS_CI_int1_theme_i)						rv = "%d_theme";
	else if(t_key == kSDS_CI_int1_stage_i)						rv = "%d_stage";
	else if(t_key == kSDS_CI_int1_reward_i)						rv = "%d_reward";
	else if(t_key == kSDS_CI_int1_missile_type_s)				rv = "%d_missile_type";
	else if(t_key == kSDS_CI_int1_missile_power_i)				rv = "%d_missile_power";
	else if(t_key == kSDS_CI_int1_missile_dex_i)				rv = "%d_missile_dex";
	else if(t_key == kSDS_CI_int1_missile_speed_d)				rv = "%d_missile_speed";
	else if(t_key == kSDS_CI_int1_passive_s)					rv = "%d_passive";
	else if(t_key == kSDS_CI_int1_abilityCnt_i)					rv = "%d_ability_cnt";
	else if(t_key == kSDS_CI_int1_ability_int2_type_i)			rv = "%d_ability_%d_type";
	else if(t_key == kSDS_CI_int1_abilityAttackOptionPower_i)	rv = "%d_ability_attack_option_power";
	else if(t_key == kSDS_CI_int1_abilityAddTimeOptionSec_i)	rv = "%d_ability_addTime_option_sec";
	else if(t_key == kSDS_CI_int1_abilityFastOptionSec_i)		rv = "%d_ability_fast_option_sec";
	else if(t_key == kSDS_CI_int1_abilitySilenceOptionSec_i)	rv = "%d_ability_silence_option_sec";
	else if(t_key == kSDS_CI_int1_abilityDoubleItemOptionPercent_i)			rv = "%d_ability_doubleItem_option_percent";
	else if(t_key == kSDS_CI_int1_abilityLongTimeOptionSec_i)				rv = "%d_ability_longTime_option_sec";
	else if(t_key == kSDS_CI_int1_abilityBaseSpeedUpOptionUnit_i)	rv = "%d_ability_baseSpeedUp_option_unit";
	else if(t_key == kSDS_CI_int1_imgInfo_s)					rv = "%d_imgInfo";
	else if(t_key == kSDS_CI_int1_thumbnailInfo_s)				rv = "%d_thumbnailInfo";
	else if(t_key == kSDS_CI_int1_aniInfoIsAni_b)				rv = "%d_aniInfo_isAni";
	else if(t_key == kSDS_CI_int1_aniInfoDetailLoopLength_i)	rv = "%d_aniInfo_detail_loopLength";
	else if(t_key == kSDS_CI_int1_aniInfoDetailLoopSeq_int2_i)	rv = "%d_aniInfo_detail_loopSeq";
	else if(t_key == kSDS_CI_int1_aniInfoDetailCutWidth_i)		rv = "%d_aniInfo_detail_cutWidth";
	else if(t_key == kSDS_CI_int1_aniInfoDetailCutHeight_i)		rv = "%d_aniInfo_detail_cutHeight";
	else if(t_key == kSDS_CI_int1_aniInfoDetailCutLength_i)		rv = "%d_aniInfo_detail_cutLength";
	else if(t_key == kSDS_CI_int1_aniInfoDetailPositionX_i)		rv = "%d_aniInfo_detail_positionX";
	else if(t_key == kSDS_CI_int1_aniInfoDetailPositionY_i)		rv = "%d_aniInfo_detail_positionY";
	else if(t_key == kSDS_CI_int1_aniInfoDetailImg_s)			rv = "%d_aniInfo_detail_img";
	else if(t_key == kSDS_CI_int1_script_s)						rv = "%d_script";
	else if(t_key == kSDS_CI_int1_silImgInfoIsSil_b)			rv = "%d_silImgInfo_isSil";
	else if(t_key == kSDS_CI_int1_silImgInfoImg_s)				rv = "%d_silImgInfo_img";
	else if(t_key == kSDS_CI_int1_profile_s)			rv = "%d_profile";
	else if(t_key == kSDS_CI_int1_name_s)			rv = "%d_name";
	else if(t_key == kSDS_CI_int1_mPrice_ruby_i)				rv = "%d_mPrice_ruby";
	else if(t_key == kSDS_CI_int1_mPrice_pass_i)				rv = "%d_mPrice_pass";
	else if(t_key == kSDS_CI_int1_type_i)						rv = "%d_type";
	else if(t_key == kSDS_CI_int1_category_s)					rv = "%d_category";
	else if(t_key == kSDS_CI_int1_level_i)						rv = "%d_level";
	else if(t_key == kSDS_CI_int1_soundCnt_i)					rv = "%d_soundCnt";
	else if(t_key == kSDS_CI_int1_soundType_int1_s)				rv = "%d_soundType_%d";
	else if(t_key == kSDS_CI_int1_characterNo_i)				rv = "%d_characterNo";
	else if(t_key == kSDS_CI_int1_serial_i)						rv = "%d_serial";
	else if(t_key == kSDS_CI_int1_haveFaceInfo_b)				rv = "%d_haveFaceInfo";
	else if(t_key == kSDS_CI_int1_faceInfo_s)					rv = "%d_faceInfo";
	else if(t_key == kSDS_CI_int1_faceInfoCcbi_s)				rv = "%d_faceInfoCcbi";
	else if(t_key == kSDS_CI_int1_faceInfoPvrccz_s)				rv = "%d_faceInfoPvrccz";
	else if(t_key == kSDS_CI_int1_faceInfoPlist_s)				rv = "%d_faceInfoPlist";
	else if(t_key == kSDS_CI_int1_haveAdult_b)					rv = "%d_haveAdult";
	else if(t_key == kSDS_CI_int1_version_i)					rv = "%d_version";
	
	//		else if(t_key == kSDS_CI_int1_silImgInfoSilData_s)			rv = "%d_silImgInfo_silData";
	
	else if(t_key == kSDS_AI_version_i)							rv = "version";
	else if(t_key == kSDS_AI_count_i)							rv = "count";
	else if(t_key == kSDS_AI_int1_id_i)							rv = "%d_id";
	else if(t_key == kSDS_AI_int1_title_s)						rv = "%d_title";
	else if(t_key == kSDS_AI_int1_content_s)					rv = "%d_content";
	else if(t_key == kSDS_AI_int1_goal_i)						rv = "%d_goal";
	else if(t_key == kSDS_AI_int1_reward_count_i)				rv = "%d_reward_count";
	else if(t_key == kSDS_AI_int1_reward_int2_type_s)			rv = "%d_reward_%d_type";
	else if(t_key == kSDS_AI_int1_reward_int2_count_i)			rv = "%d_reward_%d_count";
	else if(t_key == kSDS_AI_int1_exchangeID_s)					rv = "%d_xc";
	else if(t_key == kSDS_AI_int1_groupNo_i)					rv = "%d_gn";
	
	return rv.c_str();
}

void ServerDataSave::fFlush(SaveDataFile f_key){			myDefault->fFlush(f_key);		}
void ServerDataSave::fFlush(SaveDataFile f_key, int i1){	myDefault->fFlush(f_key, i1);	}
void ServerDataSave::fFlush(SDS_KEY fr_key)
{
	SDS_SET key_set = getKeySet(fr_key);
	fFlush(key_set.f_key);
}
void ServerDataSave::fFlush(int i1, SDS_KEY fr_key)
{
	SDS_SET key_set = getKeySet(fr_key);
	fFlush(key_set.f_key, i1);
}

SaveDataFile ServerDataSave::getSDF (SDS_KEY t_key)
{
	SaveDataFile rv;
	if(t_key >= kSDS_PZ_base && t_key <= kSDS_PZ_end)					rv = kSDF_puzzleInfo;
	else if(t_key >= kSDS_GI_base && t_key <= kSDS_GI_end)				rv = kSDF_gameInfo;
	else if(t_key >= kSDS_SI_base && t_key <= kSDS_SI_end)				rv = kSDF_stageInfo;
	else if(t_key >= kSDS_CI_base && t_key <= kSDS_CI_end)				rv = kSDF_cardInfo;
	else if(t_key >= kSDS_AI_base && t_key <= kSDS_AI_end)				rv = kSDF_achieveInfo;
	return rv;
}

void ServerDataSave::removeCache()
{
	sds_cache_bool.clear();
	sds_cache_int.clear();
	sds_cache_float.clear();
	sds_cache_double.clear();
	sds_cache_string.clear();
}

void ServerDataSave::myInit ()
{
	removeCache();
	myDefault = SaveData::sharedObject();
}
#undef LZZ_INLINE
