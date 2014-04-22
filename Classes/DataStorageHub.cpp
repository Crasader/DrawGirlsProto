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
	string return_value;
	if(t_name == kDSH_Key_notFirstExe)								return_value = "nfe";
	
	else if(t_name == kDSH_Key_bgmOff)								return_value = "bgmoff";
	else if(t_name == kDSH_Key_effectOff)							return_value = "effoff";
	else if(t_name == kDSH_Key_uiType)								return_value = "uitype";
	
	else if(t_name == kDSH_Key_savedStar)							return_value = "ss";
	else if(t_name == kDSH_Key_savedGold)							return_value = "sg";
	else if(t_name == kDSH_Key_haveItemCnt_int1)					return_value = "hic%d";
	else if(t_name == kDSH_Key_isShowItem_int1)						return_value = "isi%d";
	
	else if(t_name == kDSH_Key_heartCnt)							return_value = "hc";
	else if(t_name == kDSH_Key_heartTime)							return_value = "ht";
	
	else if(t_name == kDSH_Key_puzzleMode)							return_value = "pm";
	else if(t_name == kDSH_Key_selectedPuzzleNumber)				return_value = "spn";
	else if(t_name == kDSH_Key_lastSelectedStageForPuzzle_int1)		return_value = "lssfp%d";
	
//	else if(t_name == kDSH_Key_endPlayedStage)						return_value = "eps";
//	else if(t_name == kDSH_Key_stageClearRank_int1)					return_value = "scr%d";
	
	else if(t_name == kDSH_Key_selectedCharacter_int1_weaponSlot_int2)		return_value = "sc%dws%d"; // 0~(n-1)번째 캐릭터의 1~n번째 슬롯에 장착된 뷰티스톤의 id
	else if(t_name == kDSH_Key_selfBeautyStoneID)					return_value = "sbsid"; // 1~n 의 뷰티스톤의 id
	else if(t_name == kDSH_Key_haveBeautyStoneCnt)					return_value = "hbscnt"; // 0~n 의 뷰티스톤 개수
	else if(t_name == kDSH_Key_haveBeautyStoneID_int1)				return_value = "hbsid%d"; // 1~haveBeautyStoneCnt 의 뷰티스톤 id
	else if(t_name == kDSH_Key_beautyStoneType_int1)				return_value = "bstype%d"; // 1~selfBeautyStoneID 의 뷰티스톤 type
	else if(t_name == kDSH_Key_beautyStoneRank_int1)				return_value = "bsrank%d"; // 1~selfBeautyStoneID 의 뷰티스톤 rank
	else if(t_name == kDSH_Key_beautyStoneLevel_int1)				return_value = "bslevel%d"; // 1~selfBeautyStoneID 의 뷰티스톤 level
	
	
	else if(t_name == kDSH_Key_selectedCard)						return_value = "scard";
	else if(t_name == kDSH_Key_cardSortType)						return_value = "cst";
		
	else if(t_name == kDSH_Key_allHighScore)						return_value = "ahs";
	
	else if(t_name == kDSH_Key_jackBaseSpeed)						return_value = "jbs";
	
	else if(t_name == kDSH_Key_haveTicketCnt)						return_value = "htc";
	else if(t_name == kDSH_Key_ticketUserId_int1)					return_value = "tui%d";
	
//	else if(t_name == kDSH_Key_openStageCnt)						return_value = "osc";
//	else if(t_name == kDSH_Key_openStageNumber_int1)				return_value = "osn%d";
//	else if(t_name == kDSH_Key_isOpenStage_int1)					return_value = "ios%d";
//	else if(t_name == kDSH_Key_clearStageCnt)						return_value = "csc";
//	else if(t_name == kDSH_Key_clearStageNumber_int1)				return_value = "csn%d";
//	else if(t_name == kDSH_Key_isClearStage_int1)					return_value = "ics%d";
	
	else if(t_name == kDSH_Key_nick)								return_value = "nick";
	
	else if(t_name == kDSH_Key_wasTutorialPopupShockWave)			return_value = "wtpsw";
	else if(t_name == kDSH_Key_wasTutorialPopupMissileTrace)		return_value = "wtpmt";
	else if(t_name == kDSH_Key_wasTutorialPopupCrashArea)			return_value = "wtpca";
	
	else if(t_name == kDSH_Key_controlJoystickDirection)			return_value = "cjd";
	else if(t_name == kDSH_Key_isEnableIrregularDirection)			return_value = "idid";
	else if(t_name == kDSH_Key_isDisableDrawButton)					return_value = "iddb";
	else if(t_name == kDSH_Key_isAlwaysInvisibleJoystick)				return_value = "iavj";
	else if(t_name == kDSH_Key_isAlwaysCenterCharacter)				return_value = "iacc";
	else if(t_name == kDSH_Key_isDisableLineOver)					return_value = "idlo";
	else if(t_name == kDSH_Key_isJoystickCenterNotFixed)			return_value = "ijcnf";
	
	else if(t_name == kDSH_Key_hasShowTutorial_int1)				return_value = "hst%d";
	else if(t_name == kDSH_Key_isControlJoystickNotFixed)				return_value = "icjf";
	
	else if(t_name == kDSH_Key_selectedCharacter)					return_value = "scharacter";
	else if(t_name == kDSH_Key_weaponLevelForCharacter_int1)		return_value = "wplfc%d";
	else if(t_name == kDSH_Key_isCharacterUnlocked_int1)			return_value = "icu%d";
	
	else if(t_name == kDSH_Key_noticeViewDate_int1)					return_value = "ntcvdt%d";
	
	else if(t_name == kDSH_Key_achieveDataCnt)						return_value = "adc";
	else if(t_name == kDSH_Key_achieveData_int1_code)				return_value = "ad%dcode"; // 1 ~ cnt
	else if(t_name == kDSH_Key_achieveData_int1_value)				return_value = "ad%dvalue"; // code
	
	else if(t_name == kDSH_Key_achieve_sendHeartCnt)				return_value = "achieve_shc";
	else if(t_name == kDSH_Key_achieve_playBonusGameCnt)			return_value = "achieve_pbgc";
	else if(t_name == kDSH_Key_achieve_mapGachaCnt)					return_value = "achieve_mgc";
	else if(t_name == kDSH_Key_achieve_totalFeverCnt)				return_value = "achieve_tfc";
	else if(t_name == kDSH_Key_achieve_seqNoFailCnt)				return_value = "achieve_snfc";
	else if(t_name == kDSH_Key_achieve_catchMonsterCnt)				return_value = "achieve_cmc";
	else if(t_name == kDSH_Key_achieve_continueCnt)					return_value = "achieve_cnc";
	else if(t_name == kDSH_Key_achieve_attendanceCnt)				return_value = "achieve_ac";
	else if(t_name == kDSH_Key_achieve_changeCnt)					return_value = "achieve_chc";
	else if(t_name == kDSH_Key_achieve_failCnt)						return_value = "achieve_fc";
	else if(t_name == kDSH_Key_achieve_inviteCnt)					return_value = "achieve_ic";
	else if(t_name == kDSH_Key_achieve_challengeCnt)				return_value = "achieve_clc";
	else if(t_name == kDSH_Key_achieve_helpCnt)						return_value = "achieve_hc";
	else if(t_name == kDSH_Key_achieve_helpAcceptCnt)				return_value = "achieve_hac";
	else if(t_name == kDSH_Key_achieve_challengeAcceptCnt)			return_value = "achieve_cac";
	else if(t_name == kDSH_Key_achieve_upgradeSuccessCnt)			return_value = "achieve_usc";
	else if(t_name == kDSH_Key_achieve_perfectClearCnt)				return_value = "achieve_pcc";
	else if(t_name == kDSH_Key_achieve_seqAttendanceCnt)			return_value = "achieve_sac";
	else if(t_name == kDSH_Key_achieve_gacha1Cnt)					return_value = "achieve_g1c";
	else if(t_name == kDSH_Key_achieve_gacha2Cnt)					return_value = "achieve_g2c";
	else if(t_name == kDSH_Key_achieve_gacha3Cnt)					return_value = "achieve_g3c";
	else if(t_name == kDSH_Key_achieve_gacha4Cnt)					return_value = "achieve_g4c";
	else if(t_name == kDSH_Key_achieve_weeklyTopCnt)				return_value = "achieve_wtc";
	else if(t_name == kDSH_Key_achieve_startLuckyCnt)				return_value = "achieve_slc";
	else if(t_name == kDSH_Key_achieve_enterShopCnt)				return_value = "achieve_esc";
	else if(t_name == kDSH_Key_achieve_itemBuyCnt)					return_value = "achieve_ibc";
	
	else if(t_name == kDSH_Key_tutorial_flowStep)					return_value = "ttrl_fs";
	
	else if(t_name == kDSH_Key_minigame_playedCnt)					return_value = "mngm_pc";
	else if(t_name == kDSH_Key_minigame_int1_stageNumber)			return_value = "mngm_%d_sn";
	else if(t_name == kDSH_Key_minigame_int1_isPlayed)				return_value = "mngm_%d_ip";
	
	else if(t_name == kDSH_Key_was_opened_tutorial_dimed_achievement)	return_value = "wotd_a";
	else if(t_name == kDSH_Key_was_opened_tutorial_dimed_cardchange)	return_value = "wotd_cc";
	else if(t_name == kDSH_Key_was_opened_tutorial_dimed_cardsetting)	return_value = "wotd_cset";
	else if(t_name == kDSH_Key_was_opened_tutorial_dimed_cardstrength)	return_value = "wotd_cstr";
	else if(t_name == kDSH_Key_was_opened_tutorial_dimed_friendoption_invite)		return_value = "wotd_fi";
	else if(t_name == kDSH_Key_was_opened_tutorial_dimed_friendoption_list)			return_value = "wotd_fl";
	else if(t_name == kDSH_Key_was_opened_tutorial_dimed_friendoption_surch)		return_value = "wotd_fs";
	else if(t_name == kDSH_Key_was_opened_tutorial_dimed_main)						return_value = "wotd_m";
	else if(t_name == kDSH_Key_was_opened_tutorial_dimed_rank)						return_value = "wotd_r";
	else if(t_name == kDSH_Key_was_opened_tutorial_dimed_startsetting)				return_value = "wotd_s";
	
	else if(t_name == kDSH_Key_mission_willNeverWatch_bossLifeZero)			return_value = "mswnw_blz";
	else if(t_name == kDSH_Key_mission_willNeverWatch_subCumberCatch)		return_value = "mswnw_scc";
	else if(t_name == kDSH_Key_mission_willNeverWatch_bigArea)				return_value = "mswnw_ba";
	else if(t_name == kDSH_Key_mission_willNeverWatch_itemCollect)			return_value = "mswnw_ic";
	else if(t_name == kDSH_Key_mission_willNeverWatch_perfect)				return_value = "mswnw_pf";
	else if(t_name == kDSH_Key_mission_willNeverWatch_timeLimit)			return_value = "mswnw_tl";
	else if(t_name == kDSH_Key_mission_willNeverWatch_sequenceChange)		return_value = "mswnw_sqc";
	
	else if(t_name == kDSH_Key_storyReadPoint)								return_value = "srp";
	
	else if(t_name == kDSH_Key_isSafetyMode)								return_value = "ism";
	else if(t_name == kDSH_Key_isPushOff)									return_value = "ipo";
	else if(t_name == kDSH_Key_isMessageOff)								return_value = "imo";
	
	return return_value;
}
Json::Value DataStorageHub::getSaveAllUserDataParam ()
{
	Json::Value param;
	param["memberID"] = hspConnector::get()->getKakaoID();
	
	Json::Value data;
	
	for(int i = kSaveUserData_Key_base+1;i<kSaveUserData_Key_end;i++)
	{
		writeParamForKey(data, SaveUserData_Key(i));
	}
	
	Json::FastWriter writer;
	param["data"] = writer.write(data);
	param["nick"] = getStringForKey(kDSH_Key_nick);
	return param;
}
void DataStorageHub::loadAllUserData (Json::Value result_data)
{
	Json::Value data;
	Json::Reader reader;
	reader.parse(result_data["data"].asString(), data);
	
	CCLog("parse data : %s", GraphDogLib::JsonObjectToString(data).c_str());
	
	setIntegerForKey(kDSH_Key_savedStar, data.get(getKey(kDSH_Key_savedStar), default_ruby).asInt(), false);
	setIntegerForKey(kDSH_Key_savedGold, data.get(getKey(kDSH_Key_savedGold), default_gold).asInt(), false);
	
	setIntegerForKey(kDSH_Key_heartCnt, data[getKey(kDSH_Key_heartCnt)].asInt(), false);
	setIntegerForKey(kDSH_Key_heartTime, data[getKey(kDSH_Key_heartTime)].asInt(), false);
	
	for(int i=kIC_emptyBegin+1;i<kIC_emptyEnd;i++)
	{
		setIntegerForKey(kDSH_Key_haveItemCnt_int1, i, data[getKey(kDSH_Key_haveItemCnt_int1)][i].asInt(), false);
		setBoolForKey(kDSH_Key_isShowItem_int1, i, data[getKey(kDSH_Key_isShowItem_int1)][i].asBool(), false);
	}
	
	setIntegerForKey(kDSH_Key_allHighScore, data[getKey(kDSH_Key_allHighScore)].asInt(), false);
	setIntegerForKey(kDSH_Key_selectedCard, data[getKey(kDSH_Key_selectedCard)].asInt(), false);
	
	int have_ticket_cnt = data[getKey(kDSH_Key_haveTicketCnt)].asInt();
	setIntegerForKey(kDSH_Key_haveTicketCnt, have_ticket_cnt, false);
	for(int i=1;i<=have_ticket_cnt;i++)
		setStringForKey(kDSH_Key_ticketUserId_int1, i, data[getKey(kDSH_Key_ticketUserId_int1)][i].asString(), false);
	
//	int open_stage_cnt = data[getKey(kDSH_Key_openStageCnt)].asInt();
//	setIntegerForKey(kDSH_Key_openStageCnt, open_stage_cnt, false);
//	for(int i=1;i<=open_stage_cnt;i++)
//	{
//		int t_stage_number = data[getKey(kDSH_Key_openStageNumber_int1)][i].asInt();
//		setIntegerForKey(kDSH_Key_openStageNumber_int1, i, t_stage_number, false);
//		setBoolForKey(kDSH_Key_isOpenStage_int1, t_stage_number, data[getKey(kDSH_Key_isOpenStage_int1)][i].asBool(), false);
//	}
//	
//	int clear_stage_cnt = data[getKey(kDSH_Key_clearStageCnt)].asInt();
//	setIntegerForKey(kDSH_Key_clearStageCnt, clear_stage_cnt, false);
//	for(int i=1;i<=clear_stage_cnt;i++)
//	{
//		int t_stage_number = data[getKey(kDSH_Key_clearStageNumber_int1)][i].asInt();
//		setIntegerForKey(kDSH_Key_clearStageNumber_int1, i, t_stage_number, false);
//		setBoolForKey(kDSH_Key_isClearStage_int1, t_stage_number, data[getKey(kDSH_Key_isClearStage_int1)][i].asBool(), false);
//	}
	
	setStringForKey(kDSH_Key_nick, data[getKey(kDSH_Key_nick)].asString().c_str(), false);
	
	setIntegerForKey(kDSH_Key_selectedCharacter, data[getKey(kDSH_Key_selectedCharacter)].asInt(), false);
	for(int i=1;i<NSDS_GI(kSDS_GI_characterCount_i);i++)
	{
		bool t_unlocked = data[getKey(kDSH_Key_isCharacterUnlocked_int1)][i].asBool();
		setBoolForKey(kDSH_Key_isCharacterUnlocked_int1, i, t_unlocked, false);
	}
	for(int i=1;i<=NSDS_GI(kSDS_GI_characterCount_i);i++)
		setIntegerForKey(kDSH_Key_weaponLevelForCharacter_int1, i-1, data[getKey(kDSH_Key_weaponLevelForCharacter_int1)][i-1].asInt(), false);
	
	
	for(int i=1;i<=NSDS_GI(kSDS_GI_characterCount_i);i++)
	{
		int slot_count = NSDS_GI(kSDS_GI_characterInfo_int1_statInfo_slotCnt_i, i);
		for(int j=1;j<=slot_count;j++)
			setIntegerForKey(kDSH_Key_selectedCharacter_int1_weaponSlot_int2, i-1, j, data[getKey(kDSH_Key_selectedCharacter_int1_weaponSlot_int2)][i-1][j].asInt(), false);
	}
	
	setIntegerForKey(kDSH_Key_selfBeautyStoneID, data[getKey(kDSH_Key_selfBeautyStoneID)].asInt(), false);
	int have_beauty_stone_cnt = data[getKey(kDSH_Key_haveBeautyStoneCnt)].asInt();
	setIntegerForKey(kDSH_Key_haveBeautyStoneCnt, have_beauty_stone_cnt, false);
	for(int i=1;i<=have_beauty_stone_cnt;i++)
	{
		int beauty_stone_id = data[getKey(kDSH_Key_haveBeautyStoneID_int1)][i].asInt();
		setIntegerForKey(kDSH_Key_haveBeautyStoneID_int1, i, beauty_stone_id, false);
		setIntegerForKey(kDSH_Key_beautyStoneType_int1, beauty_stone_id, data[getKey(kDSH_Key_beautyStoneType_int1)][i].asInt(), false);
		setIntegerForKey(kDSH_Key_beautyStoneRank_int1, beauty_stone_id, data[getKey(kDSH_Key_beautyStoneRank_int1)][i].asInt(), false);
		setIntegerForKey(kDSH_Key_beautyStoneLevel_int1, beauty_stone_id, data[getKey(kDSH_Key_beautyStoneLevel_int1)][i].asInt(), false);
	}
	
	
	int achieve_data_cnt = data[getKey(kDSH_Key_achieveDataCnt)].asInt();
	setIntegerForKey(kDSH_Key_achieveDataCnt, achieve_data_cnt);
	for(int i=1;i<=achieve_data_cnt;i++)
	{
		setIntegerForKey(kDSH_Key_achieveData_int1_code, i, data[getKey(kDSH_Key_achieveData_int1_code)][i].asInt(), false);
		setIntegerForKey(kDSH_Key_achieveData_int1_value, data[getKey(kDSH_Key_achieveData_int1_code)][i].asInt(), data[getKey(kDSH_Key_achieveData_int1_value)][i].asInt(), false);
	}
	
	for(int i=kDSH_Key_achieve_base+1;i<kDSH_Key_achieve_end;i++)
		setIntegerForKey((DSH_Key)i, data[getKey((DSH_Key)i)].asInt(), false);
	
	int minigame_played_cnt = data[getKey(kDSH_Key_minigame_playedCnt)].asInt();
	setIntegerForKey(kDSH_Key_minigame_playedCnt, minigame_played_cnt, false);
	for(int i=1;i<=minigame_played_cnt;i++)
	{
		int stage_number = data[getKey(kDSH_Key_minigame_int1_stageNumber)][i].asInt();
		setIntegerForKey(kDSH_Key_minigame_int1_stageNumber, i, stage_number, false);
		setBoolForKey(kDSH_Key_minigame_int1_isPlayed, stage_number, data[getKey(kDSH_Key_minigame_int1_isPlayed)][i].asBool(), false);
	}
	
	setIntegerForKey(kDSH_Key_tutorial_flowStep, 17, false);//data[getKey(kDSH_Key_tutorial_flowStep)].asInt(), false);
	
//	int end_played_stage = data[getKey(kDSH_Key_endPlayedStage)].asInt();
//	setIntegerForKey(kDSH_Key_endPlayedStage, end_played_stage, false);
//	for(int i=1;i<=end_played_stage;i++)
//		setIntegerForKey(kDSH_Key_stageClearRank_int1, i, data[getKey(kDSH_Key_stageClearRank_int1)][i].asInt(), false);
	
	setIntegerForKey(kDSH_Key_storyReadPoint, data[getKey(kDSH_Key_storyReadPoint)].asInt(), false);
	
	fFlush();
}
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

void DataStorageHub::clear()
{
	removeCache();
	myDefault->resetData(kSDF_myDSH);
}

void DataStorageHub::resetDSH ()
{
	setIntegerForKey(kDSH_Key_savedStar, default_ruby, false);
	setIntegerForKey(kDSH_Key_savedGold, default_gold, false);
	
	setIntegerForKey(kDSH_Key_heartCnt, 0, false);
	setIntegerForKey(kDSH_Key_heartTime, 0, false);
	
	for(int i=kIC_emptyBegin+1;i<kIC_emptyEnd;i++)
	{
		setIntegerForKey(kDSH_Key_haveItemCnt_int1, i, 0, false);
		setBoolForKey(kDSH_Key_isShowItem_int1, i, false, false);
	}
	
	setIntegerForKey(kDSH_Key_allHighScore, 0, false);
	
	int have_ticket_cnt = getIntegerForKey(kDSH_Key_haveTicketCnt);
	for(int i=1;i<=have_ticket_cnt;i++)
		setStringForKey(kDSH_Key_ticketUserId_int1, i, "", false);
	setIntegerForKey(kDSH_Key_haveTicketCnt, 0, false);
	
//	int open_stage_cnt = getIntegerForKey(kDSH_Key_openStageCnt);
//	for(int i=1;i<=open_stage_cnt;i++)
//	{
//		setBoolForKey(kDSH_Key_isOpenStage_int1, getIntegerForKey(kDSH_Key_openStageNumber_int1, i), false, false);
//		setIntegerForKey(kDSH_Key_openStageNumber_int1, i, 0, false);
//	}
//	setIntegerForKey(kDSH_Key_openStageCnt, 1, false);
//	
//	
//	int clear_stage_cnt = getIntegerForKey(kDSH_Key_clearStageCnt);
//	for(int i=1;i<=clear_stage_cnt;i++)
//	{
//		setBoolForKey(kDSH_Key_isClearStage_int1, getIntegerForKey(kDSH_Key_clearStageNumber_int1, i), false, false);
//		setIntegerForKey(kDSH_Key_clearStageNumber_int1, i, 0, false);
//	}
//	setIntegerForKey(kDSH_Key_clearStageCnt, 0, false);
	
	setStringForKey(kDSH_Key_nick, "", false);
	
	setIntegerForKey(kDSH_Key_selectedCharacter, 0, false);
	for(int i=1;i<=NSDS_GI(kSDS_GI_characterCount_i);i++)
	{
		setBoolForKey(kDSH_Key_isCharacterUnlocked_int1, i, false, false);
		setIntegerForKey(kDSH_Key_weaponLevelForCharacter_int1, i-1, 0, false);
	}
	
	for(int i=1;i<=NSDS_GI(kSDS_GI_characterCount_i);i++)
	{
		int slot_count = NSDS_GI(kSDS_GI_characterInfo_int1_statInfo_slotCnt_i, i);
		for(int j=1;j<=slot_count;j++)
			setIntegerForKey(kDSH_Key_selectedCharacter_int1_weaponSlot_int2, i-1, j, 0, false);
	}
	
	setIntegerForKey(kDSH_Key_selfBeautyStoneID, 0, false);
	int have_beauty_stone_cnt = getIntegerForKey(kDSH_Key_haveBeautyStoneCnt);
	setIntegerForKey(kDSH_Key_haveBeautyStoneCnt, 0, false);
	for(int i=1;i<=have_beauty_stone_cnt;i++)
	{
		int beauty_stone_id = getIntegerForKey(kDSH_Key_haveBeautyStoneID_int1, i);
		setIntegerForKey(kDSH_Key_haveBeautyStoneID_int1, i, 0, false);
		setIntegerForKey(kDSH_Key_beautyStoneType_int1, beauty_stone_id, 0, false);
		setIntegerForKey(kDSH_Key_beautyStoneRank_int1, beauty_stone_id, 0, false);
		setIntegerForKey(kDSH_Key_beautyStoneLevel_int1, beauty_stone_id, 0, false);
	}
	
	int achieve_data_cnt = getIntegerForKey(kDSH_Key_achieveDataCnt);
	setIntegerForKey(kDSH_Key_achieveDataCnt, 0, false);
	for(int i=1;i<=achieve_data_cnt;i++)
	{
		int code = getIntegerForKey(kDSH_Key_achieveData_int1_code, i);
		setIntegerForKey(kDSH_Key_achieveData_int1_code, i, 0, false);
		setIntegerForKey(kDSH_Key_achieveData_int1_value, code, 0, false);
	}
	
	for(int i=kDSH_Key_achieve_base+1;i<kDSH_Key_achieve_end;i++)
		setIntegerForKey((DSH_Key)i, 0, false);
	
	int minigame_played_cnt = getIntegerForKey(kDSH_Key_minigame_playedCnt);
	setIntegerForKey(kDSH_Key_minigame_playedCnt, 0);
	for(int i=1;i<=minigame_played_cnt;i++)
	{
		int stage_number = getIntegerForKey(kDSH_Key_minigame_int1_stageNumber, i);
		setIntegerForKey(kDSH_Key_minigame_int1_stageNumber, i, 0, false);
		setBoolForKey(kDSH_Key_minigame_int1_isPlayed, stage_number, false, false);
	}
	
	setIntegerForKey(kDSH_Key_tutorial_flowStep, 17);
	
//	int end_played_stage = getIntegerForKey(kDSH_Key_endPlayedStage);
//	for(int i=1;i<=end_played_stage;i++)
//		setIntegerForKey(kDSH_Key_stageClearRank_int1, i, 0, false);
//	setIntegerForKey(kDSH_Key_endPlayedStage, 0);
	
	
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
	removeCache();
	myDefault = SaveData::sharedObject();
	is_cheat_key_enabled = false;
	puzzle_map_scene_show_type = kPuzzleMapSceneShowType_init;
	setIntegerForKey(kDSH_Key_tutorial_flowStep, 17);
}
#undef LZZ_INLINE
