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
	string bool_string = myDefault->getValue(kSDF_myDSH, getKey(t_key), "false");
	if(bool_string == "false")		return false;
	else							return true;
}
bool DataStorageHub::getBoolForKey (DSH_Key t_key, int key_val1)
{
	string bool_string = myDefault->getValue(kSDF_myDSH, CCString::createWithFormat(getKey(t_key).c_str(), key_val1)->getCString(), "false");
	if(bool_string == "false")		return false;
	else							return true;
}
bool DataStorageHub::getBoolForKey (DSH_Key t_key, int key_val1, int key_val2)
{
	string bool_string = myDefault->getValue(kSDF_myDSH, CCString::createWithFormat(getKey(t_key).c_str(), key_val1, key_val2)->getCString(), "false");
	if(bool_string == "false")		return false;
	else							return true;
}
void DataStorageHub::setBoolForKey (DSH_Key t_key, bool t_b, bool diskWrite)
{
	string bool_string;
	if(t_b)			bool_string = "true";
	else			bool_string = "false";
	myDefault->setKeyValue(kSDF_myDSH, getKey(t_key), bool_string, diskWrite);
}
void DataStorageHub::setBoolForKey (DSH_Key t_key, int key_val1, bool t_b, bool diskWrite)
{
	string bool_string;
	if(t_b)			bool_string = "true";
	else			bool_string = "false";
	myDefault->setKeyValue(kSDF_myDSH, CCString::createWithFormat(getKey(t_key).c_str(), key_val1)->getCString(), bool_string, diskWrite);
}
void DataStorageHub::setBoolForKey (DSH_Key t_key, int key_val1, int key_val2, bool t_b, bool diskWrite)
{
	string bool_string;
	if(t_b)			bool_string = "true";
	else			bool_string = "false";
	myDefault->setKeyValue(kSDF_myDSH, CCString::createWithFormat(getKey(t_key).c_str(), key_val1, key_val2)->getCString(), bool_string, diskWrite);
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
	return myDefault->getValue(kSDF_myDSH, getKey(t_key), 0);
}
int DataStorageHub::getIntegerForKey (DSH_Key t_key, int key_val1)
{
	return myDefault->getValue(kSDF_myDSH, CCString::createWithFormat(getKey(t_key).c_str(), key_val1)->getCString(), 0);
}
int DataStorageHub::getIntegerForKey (DSH_Key t_key, int key_val1, int key_val2)
{
	return myDefault->getValue(kSDF_myDSH, CCString::createWithFormat(getKey(t_key).c_str(), key_val1, key_val2)->getCString(), 0);
}
void DataStorageHub::setIntegerForKey (DSH_Key t_key, int val1, bool diskWrite)
{
	myDefault->setKeyValue(kSDF_myDSH, getKey(t_key), val1, diskWrite);
}
void DataStorageHub::setIntegerForKey (DSH_Key t_key, int key_val1, int val1, bool diskWrite)
{
	myDefault->setKeyValue(kSDF_myDSH, CCString::createWithFormat(getKey(t_key).c_str(), key_val1)->getCString(), val1, diskWrite);
}
string DataStorageHub::getStringForKey (DSH_Key t_key)
{
	return myDefault->getValue(kSDF_myDSH, getKey(t_key), "");
}
void DataStorageHub::setStringForKey (DSH_Key t_key, string val1, bool diskWrite)
{
	myDefault->setKeyValue(kSDF_myDSH, getKey(t_key), val1.c_str(), diskWrite);
}
string DataStorageHub::getStringForKey (DSH_Key t_key, int key_val1)
{
	return myDefault->getValue(kSDF_myDSH, CCString::createWithFormat(getKey(t_key).c_str(), key_val1)->getCString(), "");
}
void DataStorageHub::setStringForKey (DSH_Key t_key, int key_val1, string val1, bool diskWrite)
{
	myDefault->setKeyValue(kSDF_myDSH, CCString::createWithFormat(getKey(t_key).c_str(), key_val1)->getCString(), val1.c_str(), diskWrite);
}
void DataStorageHub::setIntegerForKey (DSH_Key t_key, int key_val1, int key_val2, int val1, bool diskWrite)
{
	myDefault->setKeyValue(kSDF_myDSH, CCString::createWithFormat(getKey(t_key).c_str(), key_val1, key_val2)->getCString(), val1, diskWrite);
}
CCSize DataStorageHub::getDesignResolutionSize ()
{
	return CCSizeMake(480, 320);
}
CCPoint DataStorageHub::wideWidthFixTouch (CCPoint t_p)
{	return ccp(t_p.x*mult_point.x + add_point.x, t_p.y*mult_point.y + add_point.y);	}
string DataStorageHub::getKey (DSH_Key t_name)
{
	string return_value;
	if(t_name == kDSH_Key_notFirstExe)								return_value = "nfe";
	
	else if(t_name == kDSH_Key_bgmOff)								return_value = "bgmoff";
	else if(t_name == kDSH_Key_effectOff)							return_value = "effoff";
	else if(t_name == kDSH_Key_uiType)								return_value = "uitype";
	
	else if(t_name == kDSH_Key_savedStar)							return_value = "ss";
	else if(t_name == kDSH_Key_savedGold)							return_value = "sg";
	else if(t_name == kDSH_Key_savedFriendPoint)					return_value = "sfp";
	else if(t_name == kDSH_Key_haveItemCnt_int1)					return_value = "hic%d";
	else if(t_name == kDSH_Key_isShowItem_int1)						return_value = "isi%d";
	
	else if(t_name == kDSH_Key_heartCnt)							return_value = "hc";
	else if(t_name == kDSH_Key_heartTime)							return_value = "ht";
	
	else if(t_name == kDSH_Key_puzzleMode)							return_value = "pm";
	else if(t_name == kDSH_Key_selectedPuzzleNumber)				return_value = "spn";
	else if(t_name == kDSH_Key_lastSelectedStage)					return_value = "lss";
	
	else if(t_name == kDSH_Key_selectedCard)						return_value = "scard";
	else if(t_name == kDSH_Key_selectedCardLevel)					return_value = "scardlevel";
	else if(t_name == kDSH_Key_selectedCardPassive)					return_value = "scardpassive";
	else if(t_name == kDSH_Key_cardSortType)						return_value = "cst";
	else if(t_name == kDSH_Key_inputTextCard_int1)					return_value = "itc%d";
	
	else if(t_name == kDSH_Key_cardLevel_int1)						return_value = "cl%d";
	else if(t_name == kDSH_Key_cardMaxDurability_int1)				return_value = "cmd%d";
	else if(t_name == kDSH_Key_cardPassive_int1)					return_value = "cp%d";
	
	else if(t_name == kDSH_Key_cardDurability_int1)					return_value = "cd%d";	// arg int(card_number) // return usable durability
	else if(t_name == kDSH_Key_hasGottenCard_int1)					return_value = "hgcard%d";	// arg int(card_number) // return cardTakeCnt_number
	else if(t_name == kDSH_Key_cardTakeCnt)							return_value = "ctc";	// hasGottenCards count
	else if(t_name == kDSH_Key_takeCardNumber_int1)					return_value = "tcn%d"; // arg int(loop start 1 ~ cardTakeCnt) // return card_number
	
	else if(t_name == kDSH_Key_allHighScore)						return_value = "ahs";
	
	else if(t_name == kDSH_Key_jackBaseSpeed)						return_value = "jbs";
	
	else if(t_name == kDSH_Key_openPuzzleCnt)						return_value = "opc";
	else if(t_name == kDSH_Key_isClearedPuzzle_int1)				return_value = "icp%d";
	
	else if(t_name == kDSH_Key_haveTicketCnt)						return_value = "htc";
	else if(t_name == kDSH_Key_ticketUserId_int1)					return_value = "tui%d";
	
	else if(t_name == kDSH_Key_openStageCnt)						return_value = "osc";
	else if(t_name == kDSH_Key_openStageNumber_int1)				return_value = "osn%d";
	else if(t_name == kDSH_Key_isOpenStage_int1)					return_value = "ios%d";
	else if(t_name == kDSH_Key_clearStageCnt)						return_value = "csc";
	else if(t_name == kDSH_Key_clearStageNumber_int1)				return_value = "csn%d";
	else if(t_name == kDSH_Key_isClearStage_int1)					return_value = "ics%d";
	
	else if(t_name == kDSH_Key_nick)								return_value = "nick";
	
	else if(t_name == kDSH_Key_wasTutorialPopupShockWave)			return_value = "wtpsw";
	else if(t_name == kDSH_Key_wasTutorialPopupMissileTrace)		return_value = "wtpmt";
	else if(t_name == kDSH_Key_wasTutorialPopupCrashArea)			return_value = "wtpca";
	
	else if(t_name == kDSH_Key_controlJoystickDirection)			return_value = "cjd";
	
	else if(t_name == kDSH_Key_hasShowTutorial_int1)				return_value = "hst%d";
	else if(t_name == kDSH_Key_isControlJoystickFixed)				return_value = "icjf";
	
	else if(t_name == kDSH_Key_selectedCharacter)					return_value = "scharacter";
	else if(t_name == kDSH_Key_isCharacterUnlocked_int1)			return_value = "icu%d";
	
	return return_value;
}
Json::Value DataStorageHub::getSaveAllUserDataParam ()
{
	Json::Value param;
	param["memberID"] = hspConnector::get()->getKakaoID();
	
	Json::Value data;
	
	for(int i = kSaveUserData_Key_star;i<=kSaveUserData_Key_character;i++)
	{
		writeParamForKey(data, SaveUserData_Key(i));
	}
	
	Json::FastWriter writer;
	param["data"] = writer.write(data);
	param["nick"] = getStringForKey(kDSH_Key_nick);
	return param;
}
void DataStorageHub::loadAllUserData (Json::Value result_data, vector <int> & card_data_load_list)
{
	Json::Value data;
	Json::Reader reader;
	reader.parse(result_data["data"].asString(), data);
	
	CCLog("parse data : %s", GraphDogLib::JsonObjectToString(data).c_str());
	
	setIntegerForKey(kDSH_Key_savedStar, data[getKey(kDSH_Key_savedStar)].asInt(), false);
	setIntegerForKey(kDSH_Key_savedGold, data[getKey(kDSH_Key_savedGold)].asInt(), false);
	setIntegerForKey(kDSH_Key_savedFriendPoint, data[getKey(kDSH_Key_savedFriendPoint)].asInt(), false);
	
	for(int i=kIC_attack;i<=kIC_rentCard;i++)
	{
		setIntegerForKey(kDSH_Key_haveItemCnt_int1, i, data[getKey(kDSH_Key_haveItemCnt_int1)][i].asInt(), false);
		setBoolForKey(kDSH_Key_isShowItem_int1, i, data[getKey(kDSH_Key_isShowItem_int1)][i].asBool(), false);
	}
	
	setIntegerForKey(kDSH_Key_cardTakeCnt, data[getKey(kDSH_Key_cardTakeCnt)].asInt(), false);
	int card_take_cnt = getIntegerForKey(kDSH_Key_cardTakeCnt);
	
	for(int i=1;i<=card_take_cnt;i++)
	{
		int take_card_number = data[getKey(kDSH_Key_takeCardNumber_int1)][i].asInt();
		setIntegerForKey(kDSH_Key_takeCardNumber_int1, i, take_card_number, false);
		setStringForKey(kDSH_Key_inputTextCard_int1, take_card_number, data[getKey(kDSH_Key_inputTextCard_int1)][i].asString(), false);
		setIntegerForKey(kDSH_Key_cardDurability_int1, take_card_number, data[getKey(kDSH_Key_cardDurability_int1)][i].asInt(), false);
		setIntegerForKey(kDSH_Key_hasGottenCard_int1, take_card_number, data[getKey(kDSH_Key_hasGottenCard_int1)][i].asInt(), false);
		
		setIntegerForKey(kDSH_Key_cardLevel_int1, take_card_number,
										 data[getKey(kDSH_Key_cardLevel_int1)].get(i, 1).asInt(),
										 false);
		setIntegerForKey(kDSH_Key_cardMaxDurability_int1, take_card_number,
										 data[getKey(kDSH_Key_cardMaxDurability_int1)].get(i, 5).asInt(),
										 false);
		setStringForKey(kDSH_Key_cardPassive_int1, take_card_number,
										data[getKey(kDSH_Key_cardPassive_int1)].get(i, "").asString(),
										false);
		
		if(NSDS_GS(kSDS_CI_int1_imgInfo_s, take_card_number) == "")
			card_data_load_list.push_back(take_card_number);
	}
	
	setIntegerForKey(kDSH_Key_allHighScore, data[getKey(kDSH_Key_allHighScore)].asInt(), false);
	setIntegerForKey(kDSH_Key_selectedCard, data[getKey(kDSH_Key_selectedCard)].asInt(), false);
	
	int open_puzzle_cnt = data[getKey(kDSH_Key_openPuzzleCnt)].asInt();
	
	setIntegerForKey(kDSH_Key_openPuzzleCnt, open_puzzle_cnt, false);
	for(int i=1;i<=open_puzzle_cnt+2 && i < data[getKey(kDSH_Key_isClearedPuzzle_int1)].size();i++)
		setBoolForKey(kDSH_Key_isClearedPuzzle_int1, i, data[getKey(kDSH_Key_isClearedPuzzle_int1)][i].asBool(), false);
	
	int have_ticket_cnt = data[getKey(kDSH_Key_haveTicketCnt)].asInt();
	setIntegerForKey(kDSH_Key_haveTicketCnt, have_ticket_cnt, false);
	for(int i=1;i<=have_ticket_cnt;i++)
		setStringForKey(kDSH_Key_ticketUserId_int1, i, data[getKey(kDSH_Key_ticketUserId_int1)][i].asString(), false);
	
	int open_stage_cnt = data[getKey(kDSH_Key_openStageCnt)].asInt();
	setIntegerForKey(kDSH_Key_openStageCnt, open_stage_cnt, false);
	for(int i=1;i<=open_stage_cnt;i++)
	{
		int t_stage_number = data[getKey(kDSH_Key_openStageNumber_int1)][i].asInt();
		setIntegerForKey(kDSH_Key_openStageNumber_int1, i, t_stage_number, false);
		setBoolForKey(kDSH_Key_isOpenStage_int1, t_stage_number, data[getKey(kDSH_Key_isOpenStage_int1)][i].asBool(), false);
	}
	
	int clear_stage_cnt = data[getKey(kDSH_Key_clearStageCnt)].asInt();
	setIntegerForKey(kDSH_Key_clearStageCnt, clear_stage_cnt, false);
	for(int i=1;i<=clear_stage_cnt;i++)
	{
		int t_stage_number = data[getKey(kDSH_Key_clearStageNumber_int1)][i].asInt();
		setIntegerForKey(kDSH_Key_clearStageNumber_int1, i, t_stage_number, false);
		setBoolForKey(kDSH_Key_isClearStage_int1, t_stage_number, data[getKey(kDSH_Key_isClearStage_int1)][i].asBool(), false);
	}
	
	setStringForKey(kDSH_Key_nick, data[getKey(kDSH_Key_nick)].asString().c_str(), false);
	
	setIntegerForKey(kDSH_Key_selectedCharacter, data[getKey(kDSH_Key_selectedCharacter)].asInt(), false);
	for(int i=1;i<NSDS_GI(kSDS_GI_characterCount_i);i++)
	{
		bool t_unlocked = data[getKey(kDSH_Key_isCharacterUnlocked_int1)][i].asBool();
		setBoolForKey(kDSH_Key_isCharacterUnlocked_int1, i, t_unlocked, false);
	}
	
	fFlush();
}
void DataStorageHub::writeParamForKey (Json::Value & data, SaveUserData_Key t_key)
{
	if(t_key == kSaveUserData_Key_star)
		data[getKey(kDSH_Key_savedStar)] = getIntegerForKey(kDSH_Key_savedStar);
	else if(t_key == kSaveUserData_Key_gold)
		data[getKey(kDSH_Key_savedGold)] = getIntegerForKey(kDSH_Key_savedGold);
	else if(t_key == kSaveUserData_Key_friendPoint)
		data[getKey(kDSH_Key_savedFriendPoint)] = getIntegerForKey(kDSH_Key_savedFriendPoint);
	else if(t_key == kSaveUserData_Key_item)
	{
		for(int i=kIC_attack;i<=kIC_rentCard;i++)
		{
			data[getKey(kDSH_Key_haveItemCnt_int1)][i] = getIntegerForKey(kDSH_Key_haveItemCnt_int1, i); // 0
			data[getKey(kDSH_Key_isShowItem_int1)][i] = getBoolForKey(kDSH_Key_isShowItem_int1, i);
		}
	}
	else if(t_key == kSaveUserData_Key_cardsInfo)
	{
		data[getKey(kDSH_Key_cardTakeCnt)] = getIntegerForKey(kDSH_Key_cardTakeCnt);
		int card_take_cnt = getIntegerForKey(kDSH_Key_cardTakeCnt);
		for(int i=1;i<=card_take_cnt;i++)
		{
			int take_card_number = getIntegerForKey(kDSH_Key_takeCardNumber_int1, i);
			data[getKey(kDSH_Key_takeCardNumber_int1)][i] = take_card_number;
			data[getKey(kDSH_Key_hasGottenCard_int1)][i] = getIntegerForKey(kDSH_Key_hasGottenCard_int1, take_card_number);
			data[getKey(kDSH_Key_cardDurability_int1)][i] = getIntegerForKey(kDSH_Key_cardDurability_int1, take_card_number);
			data[getKey(kDSH_Key_inputTextCard_int1)][i] = getStringForKey(kDSH_Key_inputTextCard_int1, take_card_number);
			
			data[getKey(kDSH_Key_cardLevel_int1)][i] = getIntegerForKey(kDSH_Key_cardLevel_int1, take_card_number);
			data[getKey(kDSH_Key_cardMaxDurability_int1)][i] = getIntegerForKey(kDSH_Key_cardMaxDurability_int1, take_card_number);
			data[getKey(kDSH_Key_cardPassive_int1)][i] = getStringForKey(kDSH_Key_cardPassive_int1, take_card_number);
		}
	}
	else if(t_key == kSaveUserData_Key_highScore)
		data[getKey(kDSH_Key_allHighScore)] = getIntegerForKey(kDSH_Key_allHighScore);
	else if(t_key == kSaveUserData_Key_selectedCard)
	{
		data[getKey(kDSH_Key_selectedCard)] = getIntegerForKey(kDSH_Key_selectedCard);
		data[getKey(kDSH_Key_selectedCardLevel)] = getIntegerForKey(kDSH_Key_cardLevel_int1, getIntegerForKey(kDSH_Key_selectedCard));
		data[getKey(kDSH_Key_selectedCardPassive)] = getStringForKey(kDSH_Key_cardPassive_int1, getIntegerForKey(kDSH_Key_selectedCard));
	}
	else if(t_key == kSaveUserData_Key_openPuzzle)
	{
		int open_puzzle_cnt = getIntegerForKey(kDSH_Key_openPuzzleCnt);
		data[getKey(kDSH_Key_openPuzzleCnt)] = open_puzzle_cnt;
		for(int i=1;i<=open_puzzle_cnt+2;i++)
			data[getKey(kDSH_Key_isClearedPuzzle_int1)][i] = getBoolForKey(kDSH_Key_isClearedPuzzle_int1, i);
	}
	else if(t_key == kSaveUserData_Key_haveTicket)
	{
		int have_ticket_cnt = getIntegerForKey(kDSH_Key_haveTicketCnt);
		data[getKey(kDSH_Key_haveTicketCnt)] = have_ticket_cnt;
		for(int i=1;i<=have_ticket_cnt;i++)
			data[getKey(kDSH_Key_ticketUserId_int1)][i] = getStringForKey(kDSH_Key_ticketUserId_int1, i);
	}
	else if(t_key == kSaveUserData_Key_openStage)
	{
		int open_stage_cnt = getIntegerForKey(kDSH_Key_openStageCnt);
		data[getKey(kDSH_Key_openStageCnt)] = open_stage_cnt;
		for(int i=1;i<=open_stage_cnt;i++)
		{
			int t_stage_number = getIntegerForKey(kDSH_Key_openStageNumber_int1, i);
			data[getKey(kDSH_Key_openStageNumber_int1)][i] = t_stage_number;
			data[getKey(kDSH_Key_isOpenStage_int1)][i] = getBoolForKey(kDSH_Key_isOpenStage_int1, t_stage_number);
		}
		
		int clear_stage_cnt = getIntegerForKey(kDSH_Key_clearStageCnt);
		data[getKey(kDSH_Key_clearStageCnt)] = clear_stage_cnt;
		for(int i=1;i<=clear_stage_cnt;i++)
		{
			int t_stage_number = getIntegerForKey(kDSH_Key_clearStageNumber_int1, i);
			data[getKey(kDSH_Key_clearStageNumber_int1)][i] = t_stage_number;
			data[getKey(kDSH_Key_isClearStage_int1)][i] = getBoolForKey(kDSH_Key_isClearStage_int1, t_stage_number);
		}
	}
	else if(t_key == kSaveUserData_Key_nick)
	{
		data[getKey(kDSH_Key_nick)] = getStringForKey(kDSH_Key_nick);
	}
	else if(t_key == kSaveUserData_Key_character)
	{
		data[getKey(kDSH_Key_selectedCharacter)] = getIntegerForKey(kDSH_Key_selectedCharacter);
		for(int i=2;i<=NSDS_GI(kSDS_GI_characterCount_i);i++)
			data[getKey(kDSH_Key_isCharacterUnlocked_int1)][i] = getBoolForKey(kDSH_Key_isCharacterUnlocked_int1, i);
	}
}
void DataStorageHub::saveUserData (vector <SaveUserData_Key> const & key_list, function <void(Json::Value)> t_selector)
{
	Json::Value param;
	param["memberID"] = hspConnector::get()->getKakaoID();
	
	Json::Value data;
	
	for(int i=0;i<key_list.size();i++)
	{
		writeParamForKey(data, key_list[i]);
	}
	
	Json::FastWriter writer;
	param["data"] = writer.write(data);
	param["nick"] = getStringForKey(kDSH_Key_nick);
	
	hspConnector::get()->command("updateUserData", param, t_selector);
}
void DataStorageHub::saveAllUserData (jsonSelType t_saved)
{
	hspConnector::get()->command("updateUserData", getSaveAllUserDataParam(), t_saved);
}
void DataStorageHub::resetDSH ()
{
	setIntegerForKey(kDSH_Key_savedStar, default_ruby, false);
	setIntegerForKey(kDSH_Key_savedGold, default_gold, false);
	setIntegerForKey(kDSH_Key_savedFriendPoint, 0, false);
	
	for(int i=kIC_attack;i<=kIC_rentCard;i++)
	{
		setIntegerForKey(kDSH_Key_haveItemCnt_int1, i, 0, false);
		setBoolForKey(kDSH_Key_isShowItem_int1, i, false, false);
	}
	
	int card_take_cnt = getIntegerForKey(kDSH_Key_cardTakeCnt);
	for(int i=1;i<=card_take_cnt;i++)
	{
		int take_card_number = getIntegerForKey(kDSH_Key_takeCardNumber_int1, i);
		setIntegerForKey(kDSH_Key_takeCardNumber_int1, i, 0, false);
		setIntegerForKey(kDSH_Key_hasGottenCard_int1, take_card_number, 0, false);
		setIntegerForKey(kDSH_Key_cardDurability_int1, take_card_number, 0, false);
		setStringForKey(kDSH_Key_inputTextCard_int1, take_card_number, "", false);
		
		
		setIntegerForKey(kDSH_Key_cardLevel_int1, take_card_number, 1, false);
		setIntegerForKey(kDSH_Key_cardMaxDurability_int1, take_card_number, 0, false);
		setStringForKey(kDSH_Key_cardPassive_int1, take_card_number, "", false);
	}
	setIntegerForKey(kDSH_Key_cardTakeCnt, 0, false);
	
	setIntegerForKey(kDSH_Key_allHighScore, 0, false);
	
	int opened_puzzle_cnt = getIntegerForKey(kDSH_Key_openPuzzleCnt);
	for(int i=1;i<=opened_puzzle_cnt+2;i++)
		setBoolForKey(kDSH_Key_isClearedPuzzle_int1, i, false, false);
	setIntegerForKey(kDSH_Key_openPuzzleCnt, 0, false);
	
	int have_ticket_cnt = getIntegerForKey(kDSH_Key_haveTicketCnt);
	for(int i=1;i<=have_ticket_cnt;i++)
		setStringForKey(kDSH_Key_ticketUserId_int1, i, "", false);
	setIntegerForKey(kDSH_Key_haveTicketCnt, 0, false);
	
	int open_stage_cnt = getIntegerForKey(kDSH_Key_openStageCnt);
	for(int i=1;i<=open_stage_cnt;i++)
	{
		setBoolForKey(kDSH_Key_isOpenStage_int1, getIntegerForKey(kDSH_Key_openStageNumber_int1, i), false, false);
		setIntegerForKey(kDSH_Key_openStageNumber_int1, i, 0, false);
	}
	setIntegerForKey(kDSH_Key_openStageCnt, 1, false);
	
	
	int clear_stage_cnt = getIntegerForKey(kDSH_Key_clearStageCnt);
	for(int i=1;i<=clear_stage_cnt;i++)
	{
		setBoolForKey(kDSH_Key_isClearStage_int1, getIntegerForKey(kDSH_Key_clearStageNumber_int1, i), false, false);
		setIntegerForKey(kDSH_Key_clearStageNumber_int1, i, 0, false);
	}
	setIntegerForKey(kDSH_Key_clearStageCnt, 0, false);
	
	setStringForKey(kDSH_Key_nick, "", false);
	
	setIntegerForKey(kDSH_Key_selectedCharacter, 0, false);
	for(int i=2;i<=NSDS_GI(kSDS_GI_characterCount_i);i++)
		setBoolForKey(kDSH_Key_isCharacterUnlocked_int1, i, false, false);
	
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
void DataStorageHub::myInit ()
{
	myDefault = SaveData::sharedObject();
	is_cheat_key_enabled = true;
	puzzle_map_scene_show_type = kPuzzleMapSceneShowType_init;
}
#undef LZZ_INLINE
