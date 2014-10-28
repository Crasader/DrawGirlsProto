// DataStorageHub.cpp
//

#include "DataStorageHub.h"
#include "KSUtil.h"

#define LZZ_INLINE inline
DataStorageHub * DataStorageHub::sharedInstance ()
{
	static DataStorageHub* t_dsh = NULL;
	if(t_dsh == NULL)
	{
		t_dsh = new DataStorageHub();
		t_dsh->myInit();
	}
	return t_dsh;
}
bool DataStorageHub::getBoolForKey (DSH_Key t_key)
{
	string c_key = getKey(t_key);
	
	iter_bool = dsh_cache_bool.find(c_key);
	if(iter_bool != dsh_cache_bool.end())
		return iter_bool->second.getV();
	
	string bool_string = myDefault->getValue(kSDF_myDSH, c_key, "false");
	if(bool_string == "false")
	{
		dsh_cache_bool[c_key] = false;
		return false;
	}
	else
	{
		dsh_cache_bool[c_key] = true;
		return true;
	}
}
bool DataStorageHub::getBoolForKey (DSH_Key t_key, int key_val1)
{
	string c_key = CCString::createWithFormat(getKey(t_key).c_str(), key_val1)->getCString();
	
	iter_bool = dsh_cache_bool.find(c_key);
	if(iter_bool != dsh_cache_bool.end())
		return iter_bool->second.getV();
	
	string bool_string = myDefault->getValue(kSDF_myDSH, c_key, "false");
	if(bool_string == "false")
	{
		dsh_cache_bool[c_key] = false;
		return false;
	}
	else
	{
		dsh_cache_bool[c_key] = true;
		return true;
	}
}
bool DataStorageHub::getBoolForKey (DSH_Key t_key, int key_val1, int key_val2)
{
	string c_key = CCString::createWithFormat(getKey(t_key).c_str(), key_val1, key_val2)->getCString();
	
	iter_bool = dsh_cache_bool.find(c_key);
	if(iter_bool != dsh_cache_bool.end())
		return iter_bool->second.getV();
	
	string bool_string = myDefault->getValue(kSDF_myDSH, c_key, "false");
	if(bool_string == "false")
	{
		dsh_cache_bool[c_key] = false;
		return false;
	}
	else
	{
		dsh_cache_bool[c_key] = true;
		return true;
	}
}
void DataStorageHub::setBoolForKey (DSH_Key t_key, bool t_b, bool diskWrite)
{
	string bool_string;
	if(t_b)			bool_string = "true";
	else			bool_string = "false";
	
	string c_key = getKey(t_key);
	myDefault->setKeyValue(kSDF_myDSH, c_key, bool_string, diskWrite);
	dsh_cache_bool[c_key] = t_b;
}
void DataStorageHub::setBoolForKey (DSH_Key t_key, int key_val1, bool t_b, bool diskWrite)
{
	string bool_string;
	if(t_b)			bool_string = "true";
	else			bool_string = "false";
	
	string c_key = CCString::createWithFormat(getKey(t_key).c_str(), key_val1)->getCString();
	myDefault->setKeyValue(kSDF_myDSH, c_key, bool_string, diskWrite);
	dsh_cache_bool[c_key] = t_b;
}
void DataStorageHub::setBoolForKey (DSH_Key t_key, int key_val1, int key_val2, bool t_b, bool diskWrite)
{
	string bool_string;
	if(t_b)			bool_string = "true";
	else			bool_string = "false";
	
	string c_key = CCString::createWithFormat(getKey(t_key).c_str(), key_val1, key_val2)->getCString();
	myDefault->setKeyValue(kSDF_myDSH, c_key, bool_string, diskWrite);
	dsh_cache_bool[c_key] = t_b;
}
int DataStorageHub::getUserIntForStr (string t_key, int t_default)
{
	return myDefault->getValue(kSDF_myDSH, t_key, t_default);
}
void DataStorageHub::setUserIntForStr (string t_key, int t_val1, bool diskWrite)
{
	myDefault->setKeyValue(kSDF_myDSH, t_key, t_val1, diskWrite);
}
string DataStorageHub::getUserStrForStr (string t_key, string t_default)
{
	return myDefault->getValue(kSDF_myDSH, t_key, t_default);
}
void DataStorageHub::setUserStrForStr (string t_key, string t_val1, bool diskWrite)
{
	myDefault->setKeyValue(kSDF_myDSH, t_key, t_val1, diskWrite);
}
int DataStorageHub::getIntegerForKey (DSH_Key t_key)
{
	string c_key = getKey(t_key);
	
	iter_int = dsh_cache_int.find(c_key);
	if(iter_int != dsh_cache_int.end())
		return iter_int->second.getV();
	
	int return_value = myDefault->getValue(kSDF_myDSH, c_key, 0);
	dsh_cache_int[c_key] = return_value;
	return return_value;
}
int DataStorageHub::getIntegerForKeyDefault (DSH_Key t_key, int def)
{
	string c_key = getKey(t_key);
	iter_int = dsh_cache_int.find(c_key);
	if(iter_int != dsh_cache_int.end())
		return iter_int->second.getV();
	
	int return_value = myDefault->getValue(kSDF_myDSH, c_key, def);
//	dsh_cache_int[c_key] = return_value;
	return return_value;
}
int DataStorageHub::getIntegerForKey (DSH_Key t_key, int key_val1)
{
	string c_key = CCString::createWithFormat(getKey(t_key).c_str(), key_val1)->getCString();
	
	iter_int = dsh_cache_int.find(c_key);
	if(iter_int != dsh_cache_int.end())
		return iter_int->second.getV();
	
	int return_value = myDefault->getValue(kSDF_myDSH, c_key, 0);
	dsh_cache_int[c_key] = return_value;
	return return_value;
}
int DataStorageHub::getIntegerForKey (DSH_Key t_key, int key_val1, int key_val2)
{
	string c_key = CCString::createWithFormat(getKey(t_key).c_str(), key_val1, key_val2)->getCString();
	
	iter_int = dsh_cache_int.find(c_key);
	if(iter_int != dsh_cache_int.end())
		return iter_int->second.getV();
	
	int return_value = myDefault->getValue(kSDF_myDSH, c_key, 0);
	dsh_cache_int[c_key] = return_value;
	return return_value;
}
void DataStorageHub::setIntegerForKey (DSH_Key t_key, int val1, bool diskWrite)
{
	string c_key = getKey(t_key);
	
	myDefault->setKeyValue(kSDF_myDSH, c_key, val1, diskWrite);
	dsh_cache_int[c_key] = val1;
}
void DataStorageHub::setIntegerForKey (DSH_Key t_key, int key_val1, int val1, bool diskWrite)
{
	string c_key = CCString::createWithFormat(getKey(t_key).c_str(), key_val1)->getCString();
	
	myDefault->setKeyValue(kSDF_myDSH, c_key, val1, diskWrite);
	dsh_cache_int[c_key] = val1;
}
string DataStorageHub::getStringForKey (DSH_Key t_key)
{
	string c_key = getKey(t_key);
	
	iter_string = dsh_cache_string.find(c_key);
	if(iter_string != dsh_cache_string.end())
		return iter_string->second.getV();
	
	string return_value = myDefault->getValue(kSDF_myDSH, c_key, "");
	dsh_cache_string[c_key] = return_value;
	return return_value;
}
void DataStorageHub::setStringForKey (DSH_Key t_key, string val1, bool diskWrite)
{
	string c_key = getKey(t_key);

	myDefault->setKeyValue(kSDF_myDSH, c_key, val1.c_str(), diskWrite);
	dsh_cache_string[c_key] = val1;
}
string DataStorageHub::getStringForKey (DSH_Key t_key, int key_val1)
{
	string c_key = CCString::createWithFormat(getKey(t_key).c_str(), key_val1)->getCString();
	
	iter_string = dsh_cache_string.find(c_key);
	if(iter_string != dsh_cache_string.end())
		return iter_string->second.getV();
	
	string return_value = myDefault->getValue(kSDF_myDSH, c_key, "");
	dsh_cache_string[c_key] = return_value;
	return return_value;
}
void DataStorageHub::setStringForKey (DSH_Key t_key, int key_val1, string val1, bool diskWrite)
{
	string c_key = CCString::createWithFormat(getKey(t_key).c_str(), key_val1)->getCString();
	
	myDefault->setKeyValue(kSDF_myDSH, c_key, val1.c_str(), diskWrite);
	dsh_cache_string[c_key] = val1;
}
void DataStorageHub::setIntegerForKey (DSH_Key t_key, int key_val1, int key_val2, int val1, bool diskWrite)
{
	string c_key = CCString::createWithFormat(getKey(t_key).c_str(), key_val1, key_val2)->getCString();
	
	myDefault->setKeyValue(kSDF_myDSH, c_key, val1, diskWrite);
	dsh_cache_int[c_key] = val1;
}
CCSize DataStorageHub::getDesignResolutionSize ()
{
	return CCSizeMake(480, 320);
}
CCPoint DataStorageHub::wideWidthFixTouch (CCPoint t_p)
{	return ccp(t_p.x*mult_point.x + add_point.x, t_p.y*mult_point.y + add_point.y);	}
string DataStorageHub::getKey (DSH_Key t_name)
{
	return return_value[t_name];
}
//Json::Value DataStorageHub::getSaveAllUserDataParam ()
//{
//	Json::Value param;
//	param["memberID"] = hspConnector::get()->getSocialID();
//	
//	Json::Value data;
//	
//	for(int i = kSaveUserData_Key_base+1;i<kSaveUserData_Key_end;i++)
//	{
//		writeParamForKey(data, SaveUserData_Key(i));
//	}
//	
//	Json::FastWriter writer;
//	param["data"] = writer.write(data);
//	param["nick"] = getStringForKey(kDSH_Key_nick);
//	return param;
//}
//void DataStorageHub::loadAllUserData (Json::Value result_data)
//{
//	Json::Value data;
//	Json::Reader reader;
//	reader.parse(result_data["data"].asString(), data);
//	
//	CCLOG("parse data : %s", GraphDogLib::JsonObjectToString(data).c_str());
//	
//	setIntegerForKey(kDSH_Key_savedStar, data.get(getKey(kDSH_Key_savedStar), default_ruby).asInt(), false);
//	setIntegerForKey(kDSH_Key_savedGold, data.get(getKey(kDSH_Key_savedGold), default_gold).asInt(), false);
//	
//	setIntegerForKey(kDSH_Key_heartCnt, data[getKey(kDSH_Key_heartCnt)].asInt(), false);
//	setIntegerForKey(kDSH_Key_heartTime, data[getKey(kDSH_Key_heartTime)].asInt(), false);
//	
//	for(int i=kIC_emptyBegin+1;i<kIC_emptyEnd;i++)
//	{
//		setIntegerForKey(kDSH_Key_haveItemCnt_int1, i, data[getKey(kDSH_Key_haveItemCnt_int1)][i].asInt(), false);
//		setBoolForKey(kDSH_Key_isShowItem_int1, i, data[getKey(kDSH_Key_isShowItem_int1)][i].asBool(), false);
//	}
//	
//	setIntegerForKey(kDSH_Key_allHighScore, data[getKey(kDSH_Key_allHighScore)].asInt(), false);
//	setIntegerForKey(kDSH_Key_selectedCard, data[getKey(kDSH_Key_selectedCard)].asInt(), false);
//	
//	int have_ticket_cnt = data[getKey(kDSH_Key_haveTicketCnt)].asInt();
//	setIntegerForKey(kDSH_Key_haveTicketCnt, have_ticket_cnt, false);
//	for(int i=1;i<=have_ticket_cnt;i++)
//		setStringForKey(kDSH_Key_ticketUserId_int1, i, data[getKey(kDSH_Key_ticketUserId_int1)][i].asString(), false);
//	
////	int open_stage_cnt = data[getKey(kDSH_Key_openStageCnt)].asInt();
////	setIntegerForKey(kDSH_Key_openStageCnt, open_stage_cnt, false);
////	for(int i=1;i<=open_stage_cnt;i++)
////	{
////		int t_stage_number = data[getKey(kDSH_Key_openStageNumber_int1)][i].asInt();
////		setIntegerForKey(kDSH_Key_openStageNumber_int1, i, t_stage_number, false);
////		setBoolForKey(kDSH_Key_isOpenStage_int1, t_stage_number, data[getKey(kDSH_Key_isOpenStage_int1)][i].asBool(), false);
////	}
////	
////	int clear_stage_cnt = data[getKey(kDSH_Key_clearStageCnt)].asInt();
////	setIntegerForKey(kDSH_Key_clearStageCnt, clear_stage_cnt, false);
////	for(int i=1;i<=clear_stage_cnt;i++)
////	{
////		int t_stage_number = data[getKey(kDSH_Key_clearStageNumber_int1)][i].asInt();
////		setIntegerForKey(kDSH_Key_clearStageNumber_int1, i, t_stage_number, false);
////		setBoolForKey(kDSH_Key_isClearStage_int1, t_stage_number, data[getKey(kDSH_Key_isClearStage_int1)][i].asBool(), false);
////	}
//	
//	setStringForKey(kDSH_Key_nick, result_data["nick"].asString(), false);
//	setStringForKey(kDSH_Key_flag, result_data["flag"].asString(), false);
//	
//	setIntegerForKey(kDSH_Key_selectedCharacter, data[getKey(kDSH_Key_selectedCharacter)].asInt(), false);
//	for(int i=1;i<NSDS_GI(kSDS_GI_characterCount_i);i++)
//	{
//		bool t_unlocked = data[getKey(kDSH_Key_isCharacterUnlocked_int1)][i].asBool();
//		setBoolForKey(kDSH_Key_isCharacterUnlocked_int1, i, t_unlocked, false);
//	}
//	for(int i=1;i<=NSDS_GI(kSDS_GI_characterCount_i);i++)
//		setIntegerForKey(kDSH_Key_weaponLevelForCharacter_int1, i-1, data[getKey(kDSH_Key_weaponLevelForCharacter_int1)][i-1].asInt(), false);
//	
//	
//	for(int i=1;i<=NSDS_GI(kSDS_GI_characterCount_i);i++)
//	{
//		int slot_count = NSDS_GI(kSDS_GI_characterInfo_int1_statInfo_slotCnt_i, i);
//		for(int j=1;j<=slot_count;j++)
//			setIntegerForKey(kDSH_Key_selectedCharacter_int1_weaponSlot_int2, i-1, j, data[getKey(kDSH_Key_selectedCharacter_int1_weaponSlot_int2)][i-1][j].asInt(), false);
//	}
//	
//	setIntegerForKey(kDSH_Key_selfBeautyStoneID, data[getKey(kDSH_Key_selfBeautyStoneID)].asInt(), false);
//	int have_beauty_stone_cnt = data[getKey(kDSH_Key_haveBeautyStoneCnt)].asInt();
//	setIntegerForKey(kDSH_Key_haveBeautyStoneCnt, have_beauty_stone_cnt, false);
//	for(int i=1;i<=have_beauty_stone_cnt;i++)
//	{
//		int beauty_stone_id = data[getKey(kDSH_Key_haveBeautyStoneID_int1)][i].asInt();
//		setIntegerForKey(kDSH_Key_haveBeautyStoneID_int1, i, beauty_stone_id, false);
//		setIntegerForKey(kDSH_Key_beautyStoneType_int1, beauty_stone_id, data[getKey(kDSH_Key_beautyStoneType_int1)][i].asInt(), false);
//		setIntegerForKey(kDSH_Key_beautyStoneRank_int1, beauty_stone_id, data[getKey(kDSH_Key_beautyStoneRank_int1)][i].asInt(), false);
//		setIntegerForKey(kDSH_Key_beautyStoneLevel_int1, beauty_stone_id, data[getKey(kDSH_Key_beautyStoneLevel_int1)][i].asInt(), false);
//	}
//	
//	
//	int achieve_data_cnt = data[getKey(kDSH_Key_achieveDataCnt)].asInt();
//	setIntegerForKey(kDSH_Key_achieveDataCnt, achieve_data_cnt);
//	for(int i=1;i<=achieve_data_cnt;i++)
//	{
//		setIntegerForKey(kDSH_Key_achieveData_int1_code, i, data[getKey(kDSH_Key_achieveData_int1_code)][i].asInt(), false);
//		setIntegerForKey(kDSH_Key_achieveData_int1_value, data[getKey(kDSH_Key_achieveData_int1_code)][i].asInt(), data[getKey(kDSH_Key_achieveData_int1_value)][i].asInt(), false);
//	}
//	
//	for(int i=kDSH_Key_achieve_base+1;i<kDSH_Key_achieve_end;i++)
//		setIntegerForKey((DSH_Key)i, data[getKey((DSH_Key)i)].asInt(), false);
//	
//	int minigame_played_cnt = data[getKey(kDSH_Key_minigame_playedCnt)].asInt();
//	setIntegerForKey(kDSH_Key_minigame_playedCnt, minigame_played_cnt, false);
//	for(int i=1;i<=minigame_played_cnt;i++)
//	{
//		int stage_number = data[getKey(kDSH_Key_minigame_int1_stageNumber)][i].asInt();
//		setIntegerForKey(kDSH_Key_minigame_int1_stageNumber, i, stage_number, false);
//		setBoolForKey(kDSH_Key_minigame_int1_isPlayed, stage_number, data[getKey(kDSH_Key_minigame_int1_isPlayed)][i].asBool(), false);
//	}
//	
//	setIntegerForKey(kDSH_Key_tutorial_flowStep, 17, false);//data[getKey(kDSH_Key_tutorial_flowStep)].asInt(), false);
//	
////	int end_played_stage = data[getKey(kDSH_Key_endPlayedStage)].asInt();
////	setIntegerForKey(kDSH_Key_endPlayedStage, end_played_stage, false);
////	for(int i=1;i<=end_played_stage;i++)
////		setIntegerForKey(kDSH_Key_stageClearRank_int1, i, data[getKey(kDSH_Key_stageClearRank_int1)][i].asInt(), false);
//	
//	setIntegerForKey(kDSH_Key_storyReadPoint, data[getKey(kDSH_Key_storyReadPoint)].asInt(), false);
//	
//	fFlush();
//}
void DataStorageHub::writeParamForKey (Json::Value & data, SaveUserData_Key t_key)
{
	if(t_key == kSaveUserData_Key_star)
		data[getKey(kDSH_Key_savedStar)] = getIntegerForKey(kDSH_Key_savedStar);
	else if(t_key == kSaveUserData_Key_gold)
		data[getKey(kDSH_Key_savedGold)] = getIntegerForKey(kDSH_Key_savedGold);
	else if(t_key == kSaveUserData_Key_heart)
	{
		data[getKey(kDSH_Key_heartCnt)] = getIntegerForKey(kDSH_Key_heartCnt);
		data[getKey(kDSH_Key_heartTime)] = getIntegerForKey(kDSH_Key_heartTime);
	}
	else if(t_key == kSaveUserData_Key_item)
	{
		for(int i=kIC_emptyBegin+1;i<kIC_emptyEnd;i++)
		{
			data[getKey(kDSH_Key_haveItemCnt_int1)][i] = getIntegerForKey(kDSH_Key_haveItemCnt_int1, i); // 0
			data[getKey(kDSH_Key_isShowItem_int1)][i] = getBoolForKey(kDSH_Key_isShowItem_int1, i);
		}
	}
	else if(t_key == kSaveUserData_Key_beautyStone)
	{
		for(int i=1;i<=NSDS_GI(kSDS_GI_characterCount_i);i++)
		{
			int slot_count = NSDS_GI(kSDS_GI_characterInfo_int1_statInfo_slotCnt_i, i);
			for(int j=1;j<=slot_count;j++)
				data[getKey(kDSH_Key_selectedCharacter_int1_weaponSlot_int2)][i-1][j] = getIntegerForKey(kDSH_Key_selectedCharacter_int1_weaponSlot_int2, i-1, j);
		}
		
		data[getKey(kDSH_Key_selfBeautyStoneID)] = getIntegerForKey(kDSH_Key_selfBeautyStoneID);
		int have_beauty_stone_cnt = getIntegerForKey(kDSH_Key_haveBeautyStoneCnt);
		data[getKey(kDSH_Key_haveBeautyStoneCnt)] = have_beauty_stone_cnt;
		for(int i=1;i<=have_beauty_stone_cnt;i++)
		{
			int beauty_stone_id = getIntegerForKey(kDSH_Key_haveBeautyStoneID_int1, i);
			data[getKey(kDSH_Key_haveBeautyStoneID_int1)][i] = beauty_stone_id;
			data[getKey(kDSH_Key_beautyStoneType_int1)][i] = getIntegerForKey(kDSH_Key_beautyStoneType_int1, beauty_stone_id);
			data[getKey(kDSH_Key_beautyStoneRank_int1)][i] = getIntegerForKey(kDSH_Key_beautyStoneRank_int1, beauty_stone_id);
			data[getKey(kDSH_Key_beautyStoneLevel_int1)][i] = getIntegerForKey(kDSH_Key_beautyStoneLevel_int1, beauty_stone_id);
		}
	}
	else if(t_key == kSaveUserData_Key_highScore)
		data[getKey(kDSH_Key_allHighScore)] = getIntegerForKey(kDSH_Key_allHighScore);
	else if(t_key == kSaveUserData_Key_selectedCard)
	{
		data[getKey(kDSH_Key_selectedCard)] = getIntegerForKey(kDSH_Key_selectedCard);
	}
	else if(t_key == kSaveUserData_Key_haveTicket)
	{
		int have_ticket_cnt = getIntegerForKey(kDSH_Key_haveTicketCnt);
		data[getKey(kDSH_Key_haveTicketCnt)] = have_ticket_cnt;
		for(int i=1;i<=have_ticket_cnt;i++)
			data[getKey(kDSH_Key_ticketUserId_int1)][i] = getStringForKey(kDSH_Key_ticketUserId_int1, i);
	}
//	else if(t_key == kSaveUserData_Key_openStage)
//	{
//		int open_stage_cnt = getIntegerForKey(kDSH_Key_openStageCnt);
//		data[getKey(kDSH_Key_openStageCnt)] = open_stage_cnt;
//		for(int i=1;i<=open_stage_cnt;i++)
//		{
//			int t_stage_number = getIntegerForKey(kDSH_Key_openStageNumber_int1, i);
//			data[getKey(kDSH_Key_openStageNumber_int1)][i] = t_stage_number;
//			data[getKey(kDSH_Key_isOpenStage_int1)][i] = getBoolForKey(kDSH_Key_isOpenStage_int1, t_stage_number);
//		}
//		
//		int clear_stage_cnt = getIntegerForKey(kDSH_Key_clearStageCnt);
//		data[getKey(kDSH_Key_clearStageCnt)] = clear_stage_cnt;
//		for(int i=1;i<=clear_stage_cnt;i++)
//		{
//			int t_stage_number = getIntegerForKey(kDSH_Key_clearStageNumber_int1, i);
//			data[getKey(kDSH_Key_clearStageNumber_int1)][i] = t_stage_number;
//			data[getKey(kDSH_Key_isClearStage_int1)][i] = getBoolForKey(kDSH_Key_isClearStage_int1, t_stage_number);
//		}
//	}
	else if(t_key == kSaveUserData_Key_nick)
	{
		data[getKey(kDSH_Key_nick)] = getStringForKey(kDSH_Key_nick);
	}
	else if(t_key == kSaveUserData_Key_character)
	{
		data[getKey(kDSH_Key_selectedCharacter)] = getIntegerForKey(kDSH_Key_selectedCharacter);
		for(int i=1;i<=NSDS_GI(kSDS_GI_characterCount_i);i++)
		{
			data[getKey(kDSH_Key_isCharacterUnlocked_int1)][i] = getBoolForKey(kDSH_Key_isCharacterUnlocked_int1, i);
			data[getKey(kDSH_Key_weaponLevelForCharacter_int1)][i-1] = getIntegerForKey(kDSH_Key_weaponLevelForCharacter_int1, i-1);
		}
	}
	else if(t_key == kSaveUserData_Key_achieve)
	{
		int achieve_data_cnt = getIntegerForKey(kDSH_Key_achieveDataCnt);
		data[getKey(kDSH_Key_achieveDataCnt)] = achieve_data_cnt;
		for(int i=1;i<=achieve_data_cnt;i++)
		{
			data[getKey(kDSH_Key_achieveData_int1_code)][i] = getIntegerForKey(kDSH_Key_achieveData_int1_code, i);
			data[getKey(kDSH_Key_achieveData_int1_value)][i] = getIntegerForKey(kDSH_Key_achieveData_int1_value, getIntegerForKey(kDSH_Key_achieveData_int1_code, i));
		}
		
		for(int i=kDSH_Key_achieve_base+1;i<kDSH_Key_achieve_end;i++)
			data[getKey((DSH_Key)i)] = getIntegerForKey((DSH_Key)i);
	}
	else if(t_key == kSaveUserData_Key_minigame)
	{
		int minigame_played_cnt = getIntegerForKey(kDSH_Key_minigame_playedCnt);
		data[getKey(kDSH_Key_minigame_playedCnt)] = minigame_played_cnt;
		for(int i=1;i<=minigame_played_cnt;i++)
		{
			int stage_number = getIntegerForKey(kDSH_Key_minigame_int1_stageNumber, i);
			data[getKey(kDSH_Key_minigame_int1_stageNumber)][i] = stage_number;
			data[getKey(kDSH_Key_minigame_int1_isPlayed)][i] = getBoolForKey(kDSH_Key_minigame_int1_isPlayed, stage_number);
		}
	}
	else if(t_key == kSaveUserData_Key_tutorial)
	{
		data[getKey(kDSH_Key_tutorial_flowStep)] = getIntegerForKey(kDSH_Key_tutorial_flowStep);
		data[getKey(kDSH_Key_storyReadPoint)] = getIntegerForKey(kDSH_Key_storyReadPoint);
	}
//	else if(t_key == kSaveUserData_Key_stageRank)
//	{
//		int end_played_stage = getIntegerForKey(kDSH_Key_endPlayedStage);
//		data[getKey(kDSH_Key_endPlayedStage)] = end_played_stage;
//		for(int i=1;i<=end_played_stage;i++)
//			data[getKey(kDSH_Key_stageClearRank_int1)][i] = getIntegerForKey(kDSH_Key_stageClearRank_int1, i);
//	}
}
//void DataStorageHub::saveUserData (vector <SaveUserData_Key> const & key_list, function <void(Json::Value)> t_selector)
//{
//	Json::Value param;
//	param["memberID"] = hspConnector::get()->getSocialID();
//	
//	Json::Value data;
//	
//	for(int i=0;i<key_list.size();i++)
//	{
//		writeParamForKey(data, key_list[i]);
//	}
//	
//	Json::FastWriter writer;
//	param["data"] = writer.write(data);
//	param["nick"] = getStringForKey(kDSH_Key_nick);
//	
//	hspConnector::get()->command("updateUserData", param, t_selector);
//}
//void DataStorageHub::saveAllUserData (jsonSelType t_saved)
//{
//	hspConnector::get()->command("updateUserData", getSaveAllUserDataParam(), t_saved);
//}

void DataStorageHub::clear()
{
	removeCache();
	myDefault->resetData(kSDF_myDSH);
}

void DataStorageHub::resetDSH ()
{
//	setIntegerForKey(kDSH_Key_savedStar, default_ruby, false);
//	setIntegerForKey(kDSH_Key_savedGold, default_gold, false);
	
	setIntegerForKey(kDSH_Key_heartCnt, 0, false);
	setIntegerForKey(kDSH_Key_heartTime, 0, false);
	
//	for(int i=kIC_emptyBegin+1;i<kIC_emptyEnd;i++)
//	{
//		setIntegerForKey(kDSH_Key_haveItemCnt_int1, i, 0, false);
//		setBoolForKey(kDSH_Key_isShowItem_int1, i, false, false);
//	}
//	
//	setIntegerForKey(kDSH_Key_allHighScore, 0, false);
//	
//	int have_ticket_cnt = getIntegerForKey(kDSH_Key_haveTicketCnt);
//	for(int i=1;i<=have_ticket_cnt;i++)
//		setStringForKey(kDSH_Key_ticketUserId_int1, i, "", false);
//	setIntegerForKey(kDSH_Key_haveTicketCnt, 0, false);
//	
//	setStringForKey(kDSH_Key_nick, "", false);
//	
//	setIntegerForKey(kDSH_Key_selectedCharacter, 0, false);
//	for(int i=1;i<=NSDS_GI(kSDS_GI_characterCount_i);i++)
//	{
//		setBoolForKey(kDSH_Key_isCharacterUnlocked_int1, i, false, false);
//		setIntegerForKey(kDSH_Key_weaponLevelForCharacter_int1, i-1, 0, false);
//	}
//	
//	for(int i=1;i<=NSDS_GI(kSDS_GI_characterCount_i);i++)
//	{
//		int slot_count = NSDS_GI(kSDS_GI_characterInfo_int1_statInfo_slotCnt_i, i);
//		for(int j=1;j<=slot_count;j++)
//			setIntegerForKey(kDSH_Key_selectedCharacter_int1_weaponSlot_int2, i-1, j, 0, false);
//	}
//	
//	setIntegerForKey(kDSH_Key_selfBeautyStoneID, 0, false);
//	int have_beauty_stone_cnt = getIntegerForKey(kDSH_Key_haveBeautyStoneCnt);
//	setIntegerForKey(kDSH_Key_haveBeautyStoneCnt, 0, false);
//	for(int i=1;i<=have_beauty_stone_cnt;i++)
//	{
//		int beauty_stone_id = getIntegerForKey(kDSH_Key_haveBeautyStoneID_int1, i);
//		setIntegerForKey(kDSH_Key_haveBeautyStoneID_int1, i, 0, false);
//		setIntegerForKey(kDSH_Key_beautyStoneType_int1, beauty_stone_id, 0, false);
//		setIntegerForKey(kDSH_Key_beautyStoneRank_int1, beauty_stone_id, 0, false);
//		setIntegerForKey(kDSH_Key_beautyStoneLevel_int1, beauty_stone_id, 0, false);
//	}
//	
//	int achieve_data_cnt = getIntegerForKey(kDSH_Key_achieveDataCnt);
//	setIntegerForKey(kDSH_Key_achieveDataCnt, 0, false);
//	for(int i=1;i<=achieve_data_cnt;i++)
//	{
//		int code = getIntegerForKey(kDSH_Key_achieveData_int1_code, i);
//		setIntegerForKey(kDSH_Key_achieveData_int1_code, i, 0, false);
//		setIntegerForKey(kDSH_Key_achieveData_int1_value, code, 0, false);
//	}
//	
//	for(int i=kDSH_Key_achieve_base+1;i<kDSH_Key_achieve_end;i++)
//		setIntegerForKey((DSH_Key)i, 0, false);
//	
//	int minigame_played_cnt = getIntegerForKey(kDSH_Key_minigame_playedCnt);
//	setIntegerForKey(kDSH_Key_minigame_playedCnt, 0);
//	for(int i=1;i<=minigame_played_cnt;i++)
//	{
//		int stage_number = getIntegerForKey(kDSH_Key_minigame_int1_stageNumber, i);
//		setIntegerForKey(kDSH_Key_minigame_int1_stageNumber, i, 0, false);
//		setBoolForKey(kDSH_Key_minigame_int1_isPlayed, stage_number, false, false);
//	}
//	
	setIntegerForKey(kDSH_Key_tutorial_flowStep, 17);
	
	fFlush();
}
bool DataStorageHub::isCheatKeyEnable ()
{
	return is_cheat_key_enabled;
}
int DataStorageHub::getPuzzleMapSceneShowType ()
{
	return puzzle_map_scene_show_type;
}
void DataStorageHub::setPuzzleMapSceneShowType (int t_type)
{
	puzzle_map_scene_show_type = t_type;
}
void DataStorageHub::fFlush ()
{			myDefault->fFlush(kSDF_myDSH);		}
void DataStorageHub::removeCache()
{
	dsh_cache_bool.clear();
	dsh_cache_int.clear();
	dsh_cache_string.clear();
}

void DataStorageHub::myInit ()
{
//	ccb_animation_manager_count = 0;
//	manager_list.clear();
//	check_manager_list.clear();
	removeCache();
	myDefault = SaveData::sharedObject();
	is_cheat_key_enabled = false;
	puzzle_map_scene_show_type = kPuzzleMapSceneShowType_init;
	setIntegerForKey(kDSH_Key_tutorial_flowStep, 17);
	initReturnPair();
}

void DataStorageHub::initReturnPair()
{
	return_value[kDSH_Key_notFirstExe] = "nfe";
	return_value[kDSH_Key_bgmOff] = "bgmoff";
	return_value[kDSH_Key_effectOff] = "effoff";
	return_value[kDSH_Key_uiType] = "uitype";
	
	return_value[kDSH_Key_savedStar] = "ss";
	return_value[kDSH_Key_savedGold] = "sg";
	return_value[kDSH_Key_haveItemCnt_int1] = "hic%d";
	return_value[kDSH_Key_isShowItem_int1] = "isi%d";
	
	return_value[kDSH_Key_heartCnt] = "hc";
	return_value[kDSH_Key_heartTime] = "ht";
	
	return_value[kDSH_Key_puzzleMode] = "pm";
	return_value[kDSH_Key_selectedPuzzleNumber] = "spn";
	return_value[kDSH_Key_lastSelectedStageForPuzzle_int1] = "lssfp%d";
	
	//	return_value[kDSH_Key_endPlayedStage] = "eps";
	//	return_value[kDSH_Key_stageClearRank_int1] = "scr%d";
	
	return_value[kDSH_Key_selectedCharacter_int1_weaponSlot_int2] = "sc%dws%d"; // 0~(n-1)번째 캐릭터의 1~n번째 슬롯에 장착된 뷰티스톤의 id
	return_value[kDSH_Key_selfBeautyStoneID] = "sbsid"; // 1~n 의 뷰티스톤의 id
	return_value[kDSH_Key_haveBeautyStoneCnt] = "hbscnt"; // 0~n 의 뷰티스톤 개수
	return_value[kDSH_Key_haveBeautyStoneID_int1] = "hbsid%d"; // 1~haveBeautyStoneCnt 의 뷰티스톤 id
	return_value[kDSH_Key_beautyStoneType_int1] = "bstype%d"; // 1~selfBeautyStoneID 의 뷰티스톤 type
	return_value[kDSH_Key_beautyStoneRank_int1] = "bsrank%d"; // 1~selfBeautyStoneID 의 뷰티스톤 rank
	return_value[kDSH_Key_beautyStoneLevel_int1] = "bslevel%d"; // 1~selfBeautyStoneID 의 뷰티스톤 level
	
	
	return_value[kDSH_Key_selectedCard] = "scard";
	return_value[kDSH_Key_cardSortType] = "cst";
	
	return_value[kDSH_Key_allHighScore] = "ahs";
	
	return_value[kDSH_Key_jackBaseSpeed] = "jbs";
	
	return_value[kDSH_Key_haveTicketCnt] = "htc";
	return_value[kDSH_Key_ticketUserId_int1] = "tui%d";
	
	//	return_value[kDSH_Key_openStageCnt] = "osc";
	//	return_value[kDSH_Key_openStageNumber_int1] = "osn%d";
	//	return_value[kDSH_Key_isOpenStage_int1] = "ios%d";
	//	return_value[kDSH_Key_clearStageCnt] = "csc";
	//	return_value[kDSH_Key_clearStageNumber_int1] = "csn%d";
	//	return_value[kDSH_Key_isClearStage_int1] = "ics%d";
	
	return_value[kDSH_Key_nick] = "nick";
	return_value[kDSH_Key_flag] = "flag";
	
	return_value[kDSH_Key_wasTutorialPopupShockWave] = "wtpsw";
	return_value[kDSH_Key_wasTutorialPopupMissileTrace] = "wtpmt";
	return_value[kDSH_Key_wasTutorialPopupCrashArea] = "wtpca";
	
	return_value[kDSH_Key_controlJoystickDirection] = "cjd";
	return_value[kDSH_Key_isEnableIrregularDirection] = "idid";
	return_value[kDSH_Key_isDisableDrawButton] = "iddb";
	return_value[kDSH_Key_isAlwaysInvisibleJoystick] = "iavj";
	return_value[kDSH_Key_isAlwaysCenterCharacter] = "iacc";
	return_value[kDSH_Key_isDisableLineOver] = "idlo";
	return_value[kDSH_Key_isJoystickCenterNotFixed] = "ijcnf";
	
	return_value[kDSH_Key_hasShowTutorial_int1] = "hst%d";
	return_value[kDSH_Key_isControlJoystickNotFixed] = "icjf";
	
	return_value[kDSH_Key_selectedCharacter] = "scharacter";
	return_value[kDSH_Key_weaponLevelForCharacter_int1] = "wplfc%d";
	return_value[kDSH_Key_isCharacterUnlocked_int1] = "icu%d";
	
	return_value[kDSH_Key_noticeViewDate_int1] = "ntcvdt%d";
	
	return_value[kDSH_Key_achieveDataCnt] = "adc";
	return_value[kDSH_Key_achieveData_int1_code] = "ad%dcode"; // 1 ~ cnt
	return_value[kDSH_Key_achieveData_int1_value] = "ad%dvalue"; // code
	
	return_value[kDSH_Key_achieve_sendHeartCnt] = "achieve_shc";
	return_value[kDSH_Key_achieve_playBonusGameCnt] = "achieve_pbgc";
	return_value[kDSH_Key_achieve_mapGachaCnt] = "achieve_mgc";
	return_value[kDSH_Key_achieve_totalFeverCnt] = "achieve_tfc";
	return_value[kDSH_Key_achieve_seqNoFailCnt] = "achieve_snfc";
	return_value[kDSH_Key_achieve_catchMonsterCnt] = "achieve_cmc";
	return_value[kDSH_Key_achieve_continueCnt] = "achieve_cnc";
	return_value[kDSH_Key_achieve_attendanceCnt] = "achieve_ac";
	return_value[kDSH_Key_achieve_changeCnt] = "achieve_chc";
	return_value[kDSH_Key_achieve_failCnt] = "achieve_fc";
	return_value[kDSH_Key_achieve_inviteCnt] = "achieve_ic";
	return_value[kDSH_Key_achieve_challengeCnt] = "achieve_clc";
	return_value[kDSH_Key_achieve_helpCnt] = "achieve_hc";
	return_value[kDSH_Key_achieve_helpAcceptCnt] = "achieve_hac";
	return_value[kDSH_Key_achieve_challengeAcceptCnt] = "achieve_cac";
	return_value[kDSH_Key_achieve_upgradeSuccessCnt] = "achieve_usc";
	return_value[kDSH_Key_achieve_perfectClearCnt] = "achieve_pcc";
	return_value[kDSH_Key_achieve_seqAttendanceCnt] = "achieve_sac";
	return_value[kDSH_Key_achieve_gacha1Cnt] = "achieve_g1c";
	return_value[kDSH_Key_achieve_gacha2Cnt] = "achieve_g2c";
	return_value[kDSH_Key_achieve_gacha3Cnt] = "achieve_g3c";
	return_value[kDSH_Key_achieve_gacha4Cnt] = "achieve_g4c";
	return_value[kDSH_Key_achieve_weeklyTopCnt] = "achieve_wtc";
	return_value[kDSH_Key_achieve_startLuckyCnt] = "achieve_slc";
	return_value[kDSH_Key_achieve_enterShopCnt] = "achieve_esc";
	return_value[kDSH_Key_achieve_itemBuyCnt] = "achieve_ibc";
	
	return_value[kDSH_Key_tutorial_flowStep] = "ttrl_fs";
	
	return_value[kDSH_Key_minigame_playedCnt] = "mngm_pc";
	return_value[kDSH_Key_minigame_int1_stageNumber] = "mngm_%d_sn";
	return_value[kDSH_Key_minigame_int1_isPlayed] = "mngm_%d_ip";
	
	return_value[kDSH_Key_was_opened_tutorial_dimed_achievement] = "wotd_a";
	return_value[kDSH_Key_was_opened_tutorial_dimed_cardchange] = "wotd_cc";
	return_value[kDSH_Key_was_opened_tutorial_dimed_cardsetting] = "wotd_cset";
	return_value[kDSH_Key_was_opened_tutorial_dimed_cardstrength] = "wotd_cstr";
	return_value[kDSH_Key_was_opened_tutorial_dimed_friendoption_invite] = "wotd_fi";
	return_value[kDSH_Key_was_opened_tutorial_dimed_friendoption_list] = "wotd_fl";
	return_value[kDSH_Key_was_opened_tutorial_dimed_friendoption_surch] = "wotd_fs";
	return_value[kDSH_Key_was_opened_tutorial_dimed_main] = "wotd_m";
	return_value[kDSH_Key_was_opened_tutorial_dimed_rank] = "wotd_r";
	return_value[kDSH_Key_was_opened_tutorial_dimed_startsetting] = "wotd_s";
	
	return_value[kDSH_Key_mission_willNeverWatch_bossLifeZero] = "mswnw_blz";
	return_value[kDSH_Key_mission_willNeverWatch_subCumberCatch] = "mswnw_scc";
	return_value[kDSH_Key_mission_willNeverWatch_bigArea] = "mswnw_ba";
	return_value[kDSH_Key_mission_willNeverWatch_itemCollect] = "mswnw_ic";
	return_value[kDSH_Key_mission_willNeverWatch_perfect] = "mswnw_pf";
	return_value[kDSH_Key_mission_willNeverWatch_timeLimit] = "mswnw_tl";
	return_value[kDSH_Key_mission_willNeverWatch_sequenceChange] = "mswnw_sqc";
	
	return_value[kDSH_Key_storyReadPoint] = "srp";
	
	return_value[kDSH_Key_isSafetyMode] = "ism";
	return_value[kDSH_Key_isPushOff] = "ipo";
	return_value[kDSH_Key_isMessageOff] = "imo";
	
	return_value[kDSH_Key_atTimeShowFirstPurchase] = "ats_fp";
	return_value[kDSH_Key_atTimeShowEmptyItem] = "ats_ei";
	return_value[kDSH_Key_atTimeShowStupidNpuHelp] = "ats_snh";
	return_value[kDSH_Key_atTimeShowEventRubyShop] = "ats_ers";
	return_value[kDSH_Key_atTimeShowLevelupGuide] = "ats_lg";
	return_value[kDSH_Key_stageHighRank_int1] = "stghr%d";
	return_value[kDSH_Key_showedScenario] = "ssnro";
	return_value[kDSH_Key_isShowEndlessModeTutorial] = "isemt";
	return_value[kSDH_Key_isShowOnePercentTutorial] = "issopt";
	return_value[kDSH_Key_isShowGababoTutorial] = "issgt";
	return_value[kDSH_Key_accountType] = "acct";
	
	return_value[kDSH_Key_clientVersion] = "cliv";
	return_value[kDSH_Key_dataFileCnt] = "dtfc";
	return_value[kDSH_Key_dataFileName_int1] = "dtfn%d";
	
	return_value[kDSH_Key_isCheckTerms] = "ichkt";
	return_value[kDSH_Key_savedMemberID] = "smid";
	return_value[kDSH_Key_timeZone] = "tmzn";
	
	return_value[kDSH_Key_showedKindTutorial_int1] = "skt%d";
	return_value[kDSH_Key_showedEmptyItemSale1] = "seis1";
	return_value[kDSH_Key_showedEmptyItemSale2] = "seis2";
	return_value[kDSH_Key_showedCardRankUp] = "scru";
	
	return_value[kDSH_Key_isOffCardMorphingSound] = "iocms";
	return_value[kDSH_Key_joystickSize] = "jss";
	return_value[kDSH_Key_isOffJoystickVib] = "iojv";
	return_value[kDSH_Key_isNotFirstItemGacha] = "infig";
	
	return_value[kDSH_Key_cardSettingTableOffsetDefault] = "cstod";
	return_value[kDSH_Key_cardSettingTableOffsetTake] = "cstot";
	return_value[kDSH_Key_cardSettingTableOffsetTakeReverse] = "cstotr";
	return_value[kDSH_Key_cardSettingTableOffsetGrade] = "cstog";
	return_value[kDSH_Key_cardSettingTableOffsetGradeReverse] = "cstogr";
	return_value[kDSH_Key_cardSettingTableOffsetEvent] = "cstoe";
	
	return_value[kDSH_Key_savedStartPackFirstTime] = "sspft";
	
	return_value[kDSH_Key_normalGachaList] = "nglst";
	return_value[kDSH_Key_premiumGachaList] = "pglst";
	return_value[kDSH_Key_normalGachaDataTime] = "ngdt";
	return_value[kDSH_Key_premiumGachaDataTime] = "pgdt";
}

//void DataStorageHub::changedCcbAnimationManager()
//{
//	CCLOG("CCB Animation Manager count : %d", ccb_animation_manager_count);
//}
//
//void DataStorageHub::regiCcbAnimationManager(CCBAnimationManager* t_manager)
//{
//	auto t_iter = find(manager_list.begin(), manager_list.end(), t_manager);
//	if(t_iter == manager_list.end())
//	{
//		manager_list.push_back(t_manager);
//	}
//}
//void DataStorageHub::unregiCcbAnimationManager(CCBAnimationManager* t_manager)
//{
//	auto t_iter = find(manager_list.begin(), manager_list.end(), t_manager);
//	if(t_iter != manager_list.end())
//	{
//		manager_list.erase(t_iter);
//	}
//}
//void DataStorageHub::unregiAllCcbAnimationManagers()
//{
//	vector<CCBAnimationManager*> remove_list;
//	remove_list.clear();
//	for(int i=0;i<manager_list.size();i++)
//	{
//		CCBAnimationManager* t_manager = manager_list[i];
//		auto t_iter = find(check_manager_list.begin(), check_manager_list.end(), t_manager);
//		if(t_iter != check_manager_list.end())
//		{
//			check_manager_list.erase(t_iter);
//			while(t_manager->retainCount() > 1)
//			{
//				t_manager->release();
//			}
//			remove_list.push_back(t_manager);
//			t_manager->release();
//		}
//		else
//		{
//			check_manager_list.push_back(t_manager);
//		}
//	}
//	
//	for(int i=0;i<remove_list.size();i++)
//	{
//		auto t_iter = find(manager_list.begin(), manager_list.end(), remove_list[i]);
//		if(t_iter != manager_list.end())
//			manager_list.erase(t_iter);
//	}
//}

#undef LZZ_INLINE
